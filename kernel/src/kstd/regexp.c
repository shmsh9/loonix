#include <kstd/regex.h>
void regex_automaton_free(regex_automaton *a){
    karray_free(a->alphabet);
    kfree(a);
}
void regex_match_free(regex_match_string *m){
    kfree(m->string);
    kfree(m);
}
regex_match_string *regex_match_string_new(char *s, uint32_t g, uint32_t l){
    regex_match_string *ret = kmalloc(sizeof(regex_match_string));
    *ret = (regex_match_string){
        .string = s,
        .group = g,
        .length = l
    };
    return ret;
}
regex_automaton *regex_automaton_new(uint32_t l, karray *a, uint32_t group){
   regex_automaton *ret = kmalloc(sizeof(regex_automaton));
   *ret = (regex_automaton){
       .length = l,
       .alphabet = a,
       .group = group
   };
   return ret;
}
void regex_automaton_debug_print(regex_automaton *r){
   kprintf("{ group: %d, length: %d, alphabet: { ", r->group, r->length);
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
bool _regex_match_inf_opt_at(regex_automaton *a, char **s){
    //KDEBUG("*s == %s", *s);
    return *(*s) && _karray_contains(a->alphabet, *(*s));
}
bool _regex_match_opt_at(regex_automaton *a, char **s){
    //KDEBUG("*s == %s", *s);
    (*s) += _karray_contains(a->alphabet, *(*s));
    return true;
}

bool _regex_match_fixed_at(regex_automaton *a, char **s){
    for(int i = 0; i < a->length && *(*s); i++){        
        //KDEBUG("*s == %s", *s);
        if(!_karray_contains(a->alphabet, *(*s)))
            return false;
        *s += 1;
    }
    return true;
}
karray * regex_group_join(karray *g){
    karray *ret = karray_new(sizeof(char *), kfree);
    int g0_l = 0;
    for(int i = 0; i < g->length; i++){
        g0_l += ((regex_match_string **)g->array)[i]->length;
    }
    char *g0 = kmalloc(g0_l+1);
    g0[g0_l] = 0x0;
    char *g0_curr = g0;
    for(int i = 0; i < g->length; i++){
        memcpy(g0_curr, ((regex_match_string **)g->array)[i]->string, ((regex_match_string **)g->array)[i]->length);
        g0_curr += ((regex_match_string **)g->array)[i]->length;
    }
    karray_push(ret, (uint64_t)g0);

    for(int i = 0; i < g->length; i++){
        int j = i;
        int group_l = 0;
        int group_sl = 0;
        if(!((regex_match_string **)g->array)[j]->group)
            continue;
        while(j < g->length && ((regex_match_string **)g->array)[j]->group == ((regex_match_string **)g->array)[i]->group){
            group_l++;
            group_sl += ((regex_match_string **)g->array)[j]->length;
            j++;
        }
        //KDEBUG("group_sl == %d group_l == %d", group_sl, group_l);
        char *s = kmalloc(group_sl+1);
        s[group_sl] = 0x0;
        char *curr_s = s;
        for(int k = i; k < j; k++){
            memcpy(curr_s, ((regex_match_string **)g->array)[k]->string, ((regex_match_string **)g->array)[k]->length);
            curr_s += ((regex_match_string **)g->array)[k]->length;
        }
        karray_push(ret, (uint64_t)s);
        i = j-1;
    }
    return ret;
}
karray * regex_match_group(karray *at, char *s){
    karray *ret = karray_new(sizeof(regex_match_string *), (void (*)(void *))regex_match_free);
    char *curr_s = s;
    int i = 0;
    for(; i < at->length && *curr_s; i++){
        regex_automaton *a = ((regex_automaton **)at->array)[i];
        switch(a->length){
            case REGEX_INF_ZR_LEN:{
                char *sav_currs = curr_s;
                while(_regex_match_inf_opt_at(a, &curr_s) && *curr_s){
                    //Check if end of inf loop
                    if(i+1 < at->length){
                        char *tmps = curr_s;
                        regex_automaton *tmpa = ((regex_automaton **)at->array)[i+1];
                        if(tmpa->length != REGEX_INF_ZR_LEN && tmpa->length != REGEX_OPT_LEN && _regex_match_fixed_at(tmpa, &tmps)){
                            break;
                        }
                    }
                    curr_s++;
                }
                char *match = kmalloc((curr_s - sav_currs)+1);
                match[curr_s - sav_currs] = 0x0;
                memcpy(match, sav_currs, curr_s - sav_currs);
                karray_push(
                    ret, 
                    (uint64_t)regex_match_string_new(match,a->group, curr_s - sav_currs)
                );
                break;
            }
            case REGEX_OPT_LEN:{
                char *sav_currs = curr_s;
                if(_regex_match_opt_at(a, &curr_s)){
                    char *match = kmalloc((curr_s - sav_currs)+1);
                    match[curr_s - sav_currs] = 0x0;
                    memcpy(match, sav_currs, curr_s - sav_currs);
                    karray_push(
                        ret, 
                        (uint64_t)regex_match_string_new(match,a->group, curr_s - sav_currs)
                    );
                }
                break;
            }
            default:{
                char *sav_currs = curr_s;
                if(!_regex_match_fixed_at(a, &curr_s)){
                    karray_free(ret);
                    return NULL;
                }
                char *match = kmalloc((curr_s - sav_currs)+1);
                match[curr_s - sav_currs] = 0x0;
                memcpy(match, sav_currs, curr_s - sav_currs);
                karray_push(
                    ret, 
                    (uint64_t)regex_match_string_new(match,a->group, curr_s - sav_currs)
                );
                break;
            }
        }
    }
    for(; i < at->length; i++){
        regex_automaton *a = ((regex_automaton **)at->array)[i];
        if(a->length != REGEX_INF_ZR_LEN && a->length != REGEX_OPT_LEN){
            karray_free(ret);
            return NULL;
        }
    }
    
    if(*curr_s && i != at->length){
        karray_free(ret);
        return NULL;
    }
    return ret;
}
bool regex_match(karray *at, char *s){
    char *curr_s = s;
    int i = 0;
    for(; i < at->length && *curr_s; i++){
        regex_automaton *a = ((regex_automaton **)at->array)[i];
        switch(a->length){
            case REGEX_INF_ZR_LEN:
                while(_regex_match_inf_opt_at(a, &curr_s) && *curr_s){
                    //Check if end of inf loop
                    if(i+1 < at->length){
                        char *tmps = curr_s;
                        regex_automaton *tmpa = ((regex_automaton **)at->array)[i+1];
                        if(tmpa->length != REGEX_INF_ZR_LEN && tmpa->length != REGEX_OPT_LEN && _regex_match_fixed_at(tmpa, &tmps)){
                            break;
                        }
                    }
                    curr_s++;
                }
                break;
            case REGEX_OPT_LEN:
                _regex_match_opt_at(a, &curr_s);
                break;
            default:
                if(!_regex_match_fixed_at(a, &curr_s))
                    return false;
                break;
        }
    }
    for(; i < at->length; i++){
        regex_automaton *a = ((regex_automaton **)at->array)[i];
        if(a->length != REGEX_INF_ZR_LEN && a->length != REGEX_OPT_LEN)
            return false;
    }
    return !*curr_s && i == at->length;
}
karray *regex_new(char *s){
    karray *ret = karray_new(sizeof(regex_automaton *), (void (*)(void *))regex_automaton_free);
    char whitespace_chars[] = { REGEX_WS_CHARS };
    karray *alphabet_parsed = karray_new(sizeof(char), NULL);
    uint32_t l = strlen(s);
    uint32_t group = 0;
    bool is_group = false;
    for(int i = 0; i < l; i++){
        switch(s[i]){
	        case '.':
                for(int c = 0; c < 0x100; c++)
                    karray_push(alphabet_parsed, (uint64_t)c);
                karray_push(ret, (uint64_t)regex_automaton_new(1, alphabet_parsed, is_group ? group : 0));
                alphabet_parsed = karray_new(sizeof(char), NULL);
	    	break;
	        case '?':
                ((regex_automaton **)ret->array)[ret->length-1]->length = REGEX_OPT_LEN;
	            break;
	        case '*':
                ((regex_automaton **)ret->array)[ret->length-1]->length = REGEX_INF_ZR_LEN;
	            break;
	        case '{':{
	    	    int lp = 0;
	    	    int _x = i+1;
	    	    for(; _x < l && s[_x] != '}'; _x++){
	    	        lp *=10;\
	    	        lp += s[_x] - '0';
	    	    }
	    	    ((regex_automaton **)ret->array)[ret->length-1]->length = lp;
	    	    i = _x;
	    	    break;
	        }
	        case '[':{
	    	    int _x = i+1;
	    	    for(; _x < l && s[_x] != ']'; _x++ ){
	    	        switch(s[_x]){
	    	            case '-':{
                            int dir = s[_x-1] < s[_x+1] ? 1 : -1;
                            for(char _j = s[_x-1]+dir; _j != s[_x+1]; _j+=dir)
                                karray_push(alphabet_parsed, (uint64_t)_j);
                            i++;
	    	    	        break;
                        }
	    	    	    case ',':
	    	    	        break;
	    	    	    default:
	    	    	        karray_push(alphabet_parsed, (uint64_t)s[_x]);
	    	    	        break;
	    	        }
	    	    }
	    	    i = _x;
                karray_push(ret, (uint64_t)regex_automaton_new(1, alphabet_parsed, is_group ? group : 0));
                alphabet_parsed = karray_new(sizeof(char), NULL);
	    	    break;
	        }
            case '\\':
                switch(s[i+1]){
                    case 's':
                        for(int c = 0; c < _array_len(whitespace_chars); c++)
                            karray_push(alphabet_parsed, (uint64_t)whitespace_chars[c]);
                        karray_push(ret, (uint64_t)regex_automaton_new(1, alphabet_parsed, is_group ? group : 0));
                        alphabet_parsed = karray_new(sizeof(char), NULL);
                        break;
                    default:
                        karray_push(alphabet_parsed, (uint64_t)s[i+1]);
                        karray_push(ret, (uint64_t)regex_automaton_new(1, alphabet_parsed, is_group ? group : 0));
                        alphabet_parsed = karray_new(sizeof(char), NULL);
                        break;
                }
                i++;
                break;
            case '(':
                is_group = true;
                group++;
                break;
            case ')':
                is_group = false;
                break;
            default:
                karray_push(alphabet_parsed, (uint64_t)s[i]);
                karray_push(ret, (uint64_t)regex_automaton_new(1, alphabet_parsed, is_group ? group : 0));
                alphabet_parsed = karray_new(sizeof(char), NULL);
                break;
        }
    }
    //karray_free(alphabet_parsed);
    return ret;
}
