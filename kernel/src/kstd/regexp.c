#include <kstd/regex.h>
void regex_automaton_free(regex_automaton *a){
    karray_free(a->alphabet);
    kfree(a);
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
            (s[i] > 32 && s[i] < 127) ? s[i] : ' ', 
            i+1 < r->alphabet->length ? ',' : ' '
        );
   }
   kprintf("} }\n");
}

bool regex_match(karray *at, char *s){
    regex_automaton **a = (regex_automaton **)at->array;
    int l = strlen(s);
    int k = 0;
    for(int i = 0; i < at->length; i++){
        int j = 0;
        if(a[i]->length == REGEX_OPT_LEN){
            if(_karray_contains(a[i]->alphabet, (char)s[k], cmp_char))
                k++;
            continue;
        }
        while(j < a[i]->length){
            if(a[i]->length == REGEX_INF_ZR_LEN){
                if(k >= l ){
		    //Final automaton
		    if(i+1 == at->length)
		        return true;
		    //Check if mandatory automatons missed
                    for(int k = i; k < at->length; k++){
                        if(a[k]->length != REGEX_INF_ZR_LEN || a[k]->length != REGEX_OPT_LEN){
			    return false;
			}
		    }
                    return true;

                }
                if(!_karray_contains(a[i]->alphabet, (char)s[k], cmp_char) && i+1 < at->length ){
                    if(_karray_contains(a[i+1]->alphabet, (char)s[k], cmp_char))
                        break;
                    if(a[i+1]->length == REGEX_INF_ZR_LEN)
                        break;
                }
                if(!_karray_contains(a[i]->alphabet, (char)s[k], cmp_char))
                    return false;
                j = 0;
                k++;
                continue;
            }
            if(k >= l && a[i]->length)
                return false;
            if(!_karray_contains(a[i]->alphabet, (char)s[k], cmp_char))
                return false;
            j++;
            k++;
        }
    }
    return true;
}
karray *regex_new(char *s){
    karray *ret = karray_new(sizeof(regex_automaton *), (void (*)(void *))regex_automaton_free);
    karray *special_chars = _karray_static(((char []){
        '{', '}', 
        '[', ']',
        '(',')',
        '.','*',
        '\\', '?'
    }));
    karray *special_chars_len = _karray_static(((char []){
        '{', '}','*','?'
    }));
    bool alphabet_started = false;
    bool length_parsing = false;
    uint32_t length_parsed = 0;
    bool alphabet_is_comma_list = false;
    karray *alphabet_parsed = karray_new(sizeof(char), NULL);
    uint32_t l = strlen(s);
    for(int i = 0; i < l; i++){
        switch(s[i]){
            case '\\':
                if(i+1 < l){
                    switch (s[i+1]){
                        case 's':
                            karray_push(
                                alphabet_parsed,
                                ' '
                            );
                            karray_push(
                                alphabet_parsed,
                                '\t'
                            );
                            if(i+1 >= l || (i+2 < l && !_karray_contains(special_chars, s[i+2], cmp_char)) ){
                                karray_push(
                                    ret,
                                    (uint64_t)regex_automaton_new(1, alphabet_parsed)
                                );
                                alphabet_parsed = karray_new(sizeof(char), NULL);
                            }
                            i+=1;
                            continue;
                        default:
                            karray_push(alphabet_parsed, s[i+1]);
                            break;
                    }
                }
                continue;
            case '[':
                alphabet_started = true;
                continue;
            case ']':
                alphabet_started = false;
                alphabet_is_comma_list = false;
                if(i+1 >= l || (i+1 < l && !_karray_contains(special_chars_len, s[i+1], cmp_char)) ){
                   karray_push(
                        ret,
                        (uint64_t)regex_automaton_new(1, alphabet_parsed)
                    );
                    alphabet_parsed = karray_new(sizeof(char), NULL);
                }
                continue;
            case '.':
                for(int i = 0; i < 0x100; i++){
                    karray_push(
                        alphabet_parsed,
                        (char)i
                    );
                }
                continue;
            case '*':
                karray_push(
                    ret,
                    (uint64_t)regex_automaton_new(REGEX_INF_ZR_LEN, alphabet_parsed)
                );
                alphabet_parsed = karray_new(sizeof(char), NULL);
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
            case '?':
                karray_push(
                    ret,
                    (uint64_t)regex_automaton_new(REGEX_OPT_LEN, alphabet_parsed)
                );
                alphabet_parsed = karray_new(sizeof(char), NULL);
                continue;
            default:
                if(!_karray_contains(special_chars, s[i], cmp_char) && !length_parsing && !alphabet_started){
                    karray_push(
                        alphabet_parsed,
                        (uint64_t)s[i]
                    );
		            //do not push length != 1 alphabet
                    if(i+1 < l && _karray_contains(special_chars_len, s[i+1], cmp_char))
		                continue;
		            karray_push(
                        ret,
                        (uint64_t)regex_automaton_new(1, alphabet_parsed)
                    );
                    alphabet_parsed = karray_new(sizeof(char), NULL);
                    continue;
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
                        karray_push(alphabet_parsed, (uint64_t)s[i-1]);
                        continue;
                    }
                }
        }
    }
    //FIX USE AFTER FREE HERE!
    //if(alphabet_parsed && alphabet_parsed->length == 0)
    //    karray_free(alphabet_parsed);
    return ret;
}
