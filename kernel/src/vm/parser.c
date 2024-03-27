#include <vm/parser.h>


#define SYM_SIZE 256
struct _sym_props{
    int8_t operation;
    int32_t *counter;
};

bool _check_symbols(uint8_t *in, uint64_t in_len){
    // { 'opening symbol', 'closing symbol' }
    uint8_t sym[][2] = {
        {'(',  ')'},
        {'{',  '}'},
        {'[',  ']'},
        {'\'', '\''},
        {'"',  '"'}
    };
    int32_t sym_count[SYM_SIZE] = {0};
    struct _sym_props sym_op[SYM_SIZE] = {0};
    for(uint32_t i = 0; i < sizeof(sym)/sizeof(sym[0]); i++){
        sym_op[sym[i][0]] = (struct _sym_props){
            .counter = sym_count+sym[i][0],
            .operation = +1
        };
        sym_op[sym[i][1]] = (struct _sym_props){
            //keep the same pointer for closing and opening symbols
            .counter = sym_count+sym[i][0],
            .operation = -1
        };
    }
    for(uint64_t i = 0; i < in_len; i++){
        switch (sym_op[in[i]].operation){
            case 0:
                break;
        
            default:
                *sym_op[in[i]].counter += sym_op[in[i]].operation;
                break;
        }
    }
    for(uint32_t i = 0; i < SYM_SIZE; i++)
        if(sym_count[i] != 0)
            return false;
    return true;
}


bool _map_variables(uint8_t *in, uint64_t in_len){
    khashmap *h = khashmap_new();

    for(uint64_t i = 0; i < in_len; i++){
        switch (in[i]){
        case '=':
            for(uint64_t j = i+1; j < in_len; j++){
                //pleease go smarter
                if(in[j] == '\n'){
                    vm_type t = _get_type(in+i+2, j-i);
                    if(t.type == _vm_unknown){
                        KERROR("cannot parse assignment");
                        khashmap_free(h);
                        return false;
                    }
                    //seems ok getting var name
                    for(int k = i; k > 0; k--){
                        //will not catch first line assignment
                        if(in[k] == '\n'){
                            char *name = kcalloc(i-k+1, sizeof(uint8_t));
                            memcpy(name, in+k+1, i-k-1);
                            KMESSAGE("parsed %s", name);
                            khashmap_set(h, name, 0x0);
                            kfree(name);
                            //jump to next word
                            i = j;
                            break;
                        }
                    }
                    //jump to next word
                    break;
                }
            }
            break;
        
        default:
            break;
        }
    }
    khashmap_debug_print(h);
    khashmap_free(h);
    return true;
}

bool parse_code(uint8_t *in){
    uint64_t grammars[][3] = {
        {
            (uint64_t)VM_UINT_ASSIGNMENT,
            (uint64_t)_regex_static(
                VM_STATIC_AUTOMATONS_VARIABLE_ASSIGNEMENT,
                VM_STATIC_AUTOMATONS_UINT,
                VM_STATIC_AUTOMATONS_END
            ),
            (uint64_t)"VM_UINT_ASSIGNMENT"
        },
        {
            (uint64_t)VM_UINT_DECLARATION,
            (uint64_t)_regex_static(
                VM_STATIC_AUTOMATONS_VARIABLE_DECLARATION,
                VM_STATIC_AUTOMATONS_UINT,
                VM_STATIC_AUTOMATONS_END
            ),
            (uint64_t)"VM_UINT_DECLARATION"
        },

        {
            (uint64_t)VM_STRING_ASSIGNMENT,
            (uint64_t)_regex_static(
                VM_STATIC_AUTOMATONS_VARIABLE_ASSIGNEMENT,
                VM_STATIC_AUTOMATONS_STRING,
                VM_STATIC_AUTOMATONS_END
            ),
            (uint64_t)"VM_STRING_ASSIGNMENT"
        },
        {
            (uint64_t)VM_STRING_DECLARATION,
            (uint64_t)_regex_static(
                VM_STATIC_AUTOMATONS_VARIABLE_DECLARATION,
                VM_STATIC_AUTOMATONS_STRING,
                VM_STATIC_AUTOMATONS_END
            ), 
            (uint64_t)"VM_STRING_DECLARATION"
        },
        {
            (uint64_t)VM_VARIABLE_TO_VARIABLE_ASSIGNMENT, 
            (uint64_t)_regex_static(
                VM_STATIC_AUTOMATONS_VARIABLE_ASSIGNEMENT,
                VM_STATIC_AUTOMATONS_VARIABLE_NAME,
                VM_STATIC_AUTOMATONS_END
            ), 
            (uint64_t)"VM_VARIABLE_TO_VARIABLE_ASSIGNMENT"
        },
        {
            (uint64_t)VM_VARIABLE_TO_VARIABLE_DECLARATION,
            (uint64_t)_regex_static(
                VM_STATIC_AUTOMATONS_VARIABLE_DECLARATION,
                VM_STATIC_AUTOMATONS_VARIABLE_NAME,
                VM_STATIC_AUTOMATONS_END
            ),
            (uint64_t)"VM_VARIABLE_TO_VARIABLE_DECLARATION"
        },
        {
            (uint64_t)VM_FN_DEFINITION,
            (uint64_t)_regex_static(
                VM_STATIC_AUTOMATONS_FN_DECLARATION
            ),
            (uint64_t)"VM_FN_DEFINITION"
        },

    };
    bool m = false;

    for(int i = 0; i < sizeof(grammars)/sizeof(grammars[0]); i++){
        karray *regx = (karray *)grammars[i][1];
        char *expr = (char *)grammars[i][0];
        char *name = (char *)grammars[i][2];
        if(regex_match(regx, (char *)in)){
            m = true;
            KDEBUG("%s is %s (%s)", in, name, expr);
        }
    }
    if(!m)
        KDEBUG("%s is garbage\n", in);
    for(int j = 0; j < sizeof(grammars)/sizeof(grammars[0]); j++)
        karray_free((karray *)grammars[j][1]);
    return m;
}
