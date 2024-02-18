#include <kstd/regex.h>
void regex_automaton_free(regex_automaton *a){
    karray_free(a->alphabet);
    //kfree(a);
}
regex_automaton *regex_automaton_new(uint32_t l, karray *a){
   regex_automaton *ret = kmalloc(sizeof(regex_automaton));
   *ret = (regex_automaton){
       .length = l,
       .alphabet = a
   };
   return ret;
}
void regex_automaton_debug_print(regex_automaton *r){
   kprintf("{ length: %d, alphabet: { ", r->length);
   char *s = (char *)r->alphabet->array;
   for(int i = 0; i < r->alphabet->length; i++){
       kprintf(
            "%c%c", 
            s[i], 
            i+1 < r->alphabet->length ? ',' : ' '
        );
   }
   kprintf("}\n");
}
karray *regex_new(char *s){
    karray *ret = karray_new(sizeof(regex_automaton *), (void (*)(void *))regex_automaton_free);
    //char sp[] = {'{', '}', '[', ']'};
    //karray *special_chars = _karray_static(sp);
    bool alphabet_started = false;
    bool length_parsing = false;
    uint32_t length_parsed = 0;
    bool alphabet_is_comma_list = false;
    karray *alphabet_parsed = karray_new(sizeof(char), NULL);
    uint32_t l = strlen(s);
    for(int i = 0; i < l; i++){
        switch(s[i]){
            case '[':
                alphabet_started = true;
                continue;
            case ']':
                alphabet_started = false;
                alphabet_is_comma_list = false;
                if(i+1 >= l || s[i+1] != '{'){
                   karray_push(
                        ret,
                        (uint64_t)regex_automaton_new(1, alphabet_parsed)
                    );
                    alphabet_parsed = karray_new(sizeof(char), NULL);
                }
                continue;
            case '{':
                length_parsing = true;
                length_parsed = 0;
                continue;
            case '}':
                length_parsing = false;
                karray_push(
                    ret,
                    (uint64_t)regex_automaton_new(length_parsed, alphabet_parsed)
                );
                alphabet_parsed = karray_new(sizeof(char), NULL);
                length_parsed = 0;
                continue;
            default:
                if(!length_parsing && !alphabet_started){
                    karray_push(
                        alphabet_parsed,
                        (uint64_t)s[i]
                    );
                    karray_push(
                        ret,
                        (uint64_t)regex_automaton_new(1, alphabet_parsed)
                    );
                    alphabet_parsed = karray_new(sizeof(char), NULL);
                }
                if(length_parsing){
                    length_parsed *= 10;
                    length_parsed += s[i] - '0';
                    continue;
                }
                if(alphabet_started){
                    if(alphabet_is_comma_list && s[i] != ','){
                        karray_push(alphabet_parsed, (uint64_t)s[i]);
                        continue;
                    }
                    if(s[i] == '-'){
                        for(int x = s[i-1]; x < s[i+1]+1; x++)
                            karray_push(alphabet_parsed, (uint64_t)x);
                        continue;
                    }
                    if(s[i] == ',' && !alphabet_is_comma_list){
                        alphabet_is_comma_list = true;
                        karray_push(alphabet_parsed, s[i-1]);
                        continue;
                    }
                }
        }
    }
    return ret;
}
/*
def automat(exp: str) -> list:
    special_chars = [
        "{", "}", "[", "]"
    ]
    automatons = []
    alphabet_started = False
    length_parsing = False
    length_parsed = 0
    alphabet_parsed = []
    alphabet_is_comma_list = False
    for i in range(0, len(exp)):
        #single char
        if exp[i] not in special_chars and not length_parsing and not alphabet_started:
            automatons.append({
                "alphabet": [exp[i]],
                "length": 1
            })
        #charset
        if exp[i] == "[":
            alphabet_started = True
            continue
        #end of charset
        if exp[i] == "]":
            alphabet_started = False
            alphabet_is_comma_list = False
            #single char from alphabet
            if i+1 >= len(exp) or exp[i+1] != "{":
                automatons.append({
                    "alphabet": alphabet_parsed,
                    "length": 1
                })
                alphabet_parsed = []
            continue
        if exp[i] == "{":
            length_parsing = True
            length_parsed = 0
            continue
        if exp[i] == "}":
            length_parsing = False
            automatons.append({
                "alphabet": alphabet_parsed,
                "length": length_parsed
            })
            alphabet_parsed = []
            length_parsed = 0
            continue
        if length_parsing:
            length_parsed *= 10
            length_parsed += int(exp[i])
            continue
        if alphabet_started:
            if alphabet_is_comma_list and exp[i] != ",":
                alphabet_parsed.append(exp[i])
            if exp[i] == "-":
                alphabet_parsed += [chr(x) for x in range(ord(exp[i-1]), ord(exp[i+1])+1)]
                continue
            if exp[i] == "," and not alphabet_is_comma_list:
                alphabet_is_comma_list = True
                alphabet_parsed.append(exp[i-1])
                continue
            continue
            pass
    return automatons

def automat_match(aut: list, s: str) -> bool:
    i = 0
    print(aut)
    print(s)
    for a in aut:
        print(a)
        j = 0
        while j < a["length"]:
            print(f"{s[i]} in {a['alphabet']}")
            if s[i] not in a["alphabet"]:
                return False
            i += 1
            j += 1
    return True

r = automat("regex[0-9][a-c]{3}")
print(
    automat_match(r, "regex1aaa")
)
*/


