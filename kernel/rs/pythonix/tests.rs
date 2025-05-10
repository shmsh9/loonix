use alloc::vec;
use alloc::collections::BTreeMap;
use tokenizer::{tokenize,parse};
use interpreter::{PyType, Context, ExpressionType, eval_expression_type};

pub fn tests(){
	assert_eq!(PyType::from("'abcdefg'"), Some(PyType::str("abcdefg".into())));
	assert_eq!(PyType::from("\"abcdefg\""), Some(PyType::str("abcdefg".into())));
	assert_eq!(PyType::from("123"), Some(PyType::int(123)));
	assert_eq!(PyType::from("'abcdefg''"), None);
	assert_eq!(ExpressionType::assign, eval_expression_type(&parse(&tokenize("a=1"))));
	assert_eq!(PyType::dict(BTreeMap::new()), PyType::dict(BTreeMap::new()));
	let mut ctxt = Context::new();
    ctxt.execute("a = [1]+[2]");
	assert_eq!(ctxt.state.get(&PyType::str("a".into())), Some(
		&PyType::list(vec![
			PyType::int(1), PyType::int(2)
		])
	));
	ctxt.execute("a = 1");
	assert_eq!(
		ctxt.state,
		BTreeMap::from([
			(PyType::str("a".into()), PyType::int(1))
		])
	);
	ctxt.execute("a = {\"b\": []}");
	assert_eq!(
		ctxt.state,
		BTreeMap::from([
			(PyType::str("a".into()), 
			PyType::dict(
				BTreeMap::from([
					(PyType::str("b".into()), PyType::list(vec![]))
				])
			))
		])
	);
	ctxt.execute("a = {'b': [], 'c': [1,2,3,{}]}");
	assert_eq!(
		ctxt.state,
		BTreeMap::from([
			(PyType::str("a".into()), 
			PyType::dict(
				BTreeMap::from([
					(PyType::str("b".into()), PyType::list(vec![])),
					(PyType::str("c".into()), PyType::list(vec![
						PyType::int(1), PyType::int(2), PyType::int(3), PyType::dict(BTreeMap::new())
					]))
			
				])
			))
		])
	);
	ctxt.execute("b = a");
	ctxt.execute("a = 1+3");
	assert_eq!(ctxt.state.get(&PyType::str("a".into())), Some(&PyType::int(4)));
    ctxt.execute("a = [1,2] + [3] + [4]");
	assert_eq!(ctxt.state.get(&PyType::str("a".into())), Some(
		&PyType::list(vec![
			PyType::int(1), PyType::int(2), PyType::int(3), PyType::int(4)
		])
	));
    ctxt.execute("a = [1+1,2]");
	assert_eq!(ctxt.state.get(&PyType::str("a".into())), Some(
		&PyType::list(vec![
			PyType::int(2), PyType::int(2)
		])
	));
    ctxt.execute("a = 'a'+\"b\"");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::str("ab".into()))
    );
    ctxt.execute("a = {'a'+'bcd': 'efg', 'xyz': [{},2,3]}");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::dict(
            BTreeMap::from([
                (PyType::str("abcd".into()), PyType::str("efg".into())),
                (PyType::str("xyz".into()), PyType::list(vec![
                    PyType::dict(BTreeMap::new()), PyType::int(2), PyType::int(3)
                ])),

            ])
        ))
    );
    ctxt.execute("a = [[],1,2,3]");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::list(vec![
            PyType::list(vec![]),
            PyType::int(1), 
            PyType::int(2),
            PyType::int(3)
        ]))
    );
    ctxt.execute("a = []");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::list(vec![]))
    );

    ctxt.execute("a = [{'b': {'c':1}}]");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::list(vec![
            PyType::dict(BTreeMap::from([
                (PyType::str("b".into()),
                    PyType::dict(BTreeMap::from([
                        (PyType::str("c".into()), PyType::int(1))
                    ]))
                )
            ]))
        ]))
    );
	ctxt.execute("a = {\"a\" : [{\"a\": \"b\"}]}");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::dict(BTreeMap::from([
			(PyType::str("a".into()), 
			PyType::list(vec![
				PyType::dict(BTreeMap::from([
					(PyType::str("a".into()), PyType::str("b".into()))
				]))
			]))
				
			]))
	));
	ctxt.execute("a = {\"a\" : [{\"b\": [1,2,3, {\"c\": {\"x\": [\"y\",\"z\"]}}]}], \"b\" : {\"c\"+\"d\": [1,2,3, {\"e\":\"fgh\"}]}}");
	ctxt.execute("a = 1");
	ctxt.execute("a  += 4+5");
	assert_eq!(
		ctxt.state.get(&PyType::str("a".into())),
		Some(&PyType::int(10))
	);
	ctxt.execute("a = 3-4");
	assert_eq!(
		ctxt.state.get(&PyType::str("a".into())),
		Some(&PyType::int(-1))
	);
	ctxt.execute("a -= 10");
	assert_eq!(
		ctxt.state.get(&PyType::str("a".into())),
		Some(&PyType::int(-11))
	);
	ctxt.execute("a = 1-3+4");
	assert_eq!(
		ctxt.state.get(&PyType::str("a".into())),
		Some(&PyType::int(2))
	);
    ctxt.execute("a = 1-3+4-5+10-99");
	assert_eq!(
		ctxt.state.get(&PyType::str("a".into())),
		Some(&PyType::int(-92))
	);

    ctxt.execute("a = [1] + [2+2]");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::list(vec![PyType::int(1),PyType::int(4)]))
	);
	ctxt.execute("a = 3*3");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::int(9))
	);
	ctxt.execute("a = \"a\"*9");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::str("aaaaaaaaa".into()))
	);
	ctxt.execute("a = 1-3*9");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::int(-26))
	);
	ctxt.execute("a = 1*4-35+79-49*6*4");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::int(-1128))
	);
	ctxt.execute("a = (3+4)*3");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::int(21))
	);
	ctxt.execute("a = (1+3*(3+3))");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::int(19))
	);
	ctxt.execute("a = (1+3*(3+(3*(4+5-(5*(10+1))))))");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::int(-404))
	);
	ctxt.execute("a = [-1-3]");
	assert_eq!(
        ctxt.state.get(&PyType::str("a".into())), 
        Some(&PyType::list(vec![PyType::int(-4)]))
	);
	assert_eq!(
		ctxt.execute("3+4"),
		PyType::int(7)
	);
	assert_eq!(ctxt.execute("sum([1,2,3,4])"), PyType::int(10));
	assert_eq!(
		ctxt.execute("sum([1,1,1,1,1,1,1,1,1,1])"),
		ctxt.execute("len('a'*10)")
	);
	assert_eq!(ctxt.execute("sum([len([1,2,2,5,3+40])])"), PyType::int(5));
	assert_eq!(
		ctxt.execute("[len(\"a\"*99),{sum(1,3): sum(1,3)}]"),
		PyType::list(vec![
			PyType::int(99),
			PyType::dict(BTreeMap::from([(PyType::int(4),PyType::int(4))]))
		])
	);
	assert_eq!(
		ctxt.execute("[len('a')+len('a'*99)]"),
		PyType::list(vec![PyType::int(100)])
	);
	assert_eq!(
		ctxt.execute("{len('a')+sum([1,1+1,3,4])+len(\"b\"): 0}"),
		PyType::dict(BTreeMap::from([(PyType::int(12), PyType::int(0))]))
	);
	assert_eq!(
		ctxt.execute("range(0,0xa)"),
		PyType::list((0..10).map(|i| PyType::int(i)).collect())
	);	
	assert_eq!(
		ctxt.execute("\"{}()+,[]\""),
		PyType::str("{}()+,[]".into())
	);	
	assert_eq!(
		ctxt.execute("fmt(\"sum(range(0,0xff)) == {}\", sum(range(0,0xff)))"),
		PyType::str("sum(range(0,0xff)) == 32385".into())
	);	
	assert_eq!(
		ctxt.execute("[1,2,3][1-1+1]"),
		PyType::int(2)
	);
    assert_eq!(
        ctxt.execute("{1: 1}[1]"),
        PyType::int(1)
    );
    assert_eq!(
        ctxt.execute("[1,2,[3]][2][0]"),
        PyType::int(3)
    );
    assert_eq!(
        ctxt.execute("[1,2,[3]][-1]"),
        PyType::list(vec![PyType::int(3)])
    );
    assert_eq!(
        ctxt.execute("{'a': [1,2,3,{'b':1}]}['a'][3]['b']"),
        PyType::int(1)
    );
    ctxt.execute("a = [1,2]");
    ctxt.execute("a[0] = 2");
    assert_eq!(
        ctxt.state.get(&PyType::str("a".into())).unwrap(),
        &PyType::list(vec![PyType::int(2),PyType::int(2)])
    );
}
