#include <kstd/karray.h>
void karray_lock(karray *array){
    while(array->lock){
        sleep_100(1);
    }
    array->lock++;
}
void karray_unlock(karray *array){
    array->lock--;
}
void karray_push(karray *array, uint64_t elem){
    karray_lock(array);
    if(array->length+1 > array->alloc){
        void *tmp = krealloc(array->array, ((array->alloc*array->elementsz)*2));
        if(tmp){
            kfree(array->array);
            array->array = tmp;
            array->alloc *= 2;
        }
        else{
            KERROR("krealloc() : failed");
            karray_unlock(array);
            return;
        }
    }
    switch(array->elementsz){
        case 1:
            ((uint8_t *)array->array)[array->length++] = (uint8_t)elem;
            break;
        case 2:
            ((uint16_t *)array->array)[array->length++] = (uint16_t)elem;
            break;
        case 4:
            ((uint32_t *)array->array)[array->length++] = (uint32_t)elem;
            break;
        case 8:
            ((uint64_t *)array->array)[array->length++] = (uint64_t)elem;
            break;
    }
    karray_unlock(array);
}

void karray_pop(karray *array ,uint64_t index){
    karray_lock(array);
    if(index >= array->length){ 
        KMESSAGE("index >= array->length");
        karray_unlock(array);
        return;
    }
    if(array->karray_data_free_fn){
        switch (array->elementsz){
            case 1:{
                uint8_t *casted_array = (uint8_t *)array->array;
                array->karray_data_free_fn((void*)(uintptr_t)casted_array[index]);
                break;
            }
            case 2:{
                uint16_t *casted_array = (uint16_t *)array->array;
                array->karray_data_free_fn((void*)(uintptr_t)casted_array[index]);
                break;
            }
            case 4:{
                uint32_t *casted_array = (uint32_t *)array->array;
                array->karray_data_free_fn((void*)(uintptr_t)casted_array[index]);
                break;
            }
            case 8:{
                uint64_t *casted_array = (uint64_t *)array->array;
                array->karray_data_free_fn((void*)casted_array[index]);
                break;
            }
        }
    }
    for(uint64_t i = index; i < array->length - 1; i++){
        switch (array->elementsz){
            case 1:{
                uint8_t *casted_array = (uint8_t *)array->array;
                casted_array[i] = casted_array[i+1];
                break;
            }
            case 2:{
                uint16_t *casted_array = (uint16_t *)array->array;
                casted_array[i] = casted_array[i+1];
                break;
            }
            case 4:{
                uint32_t *casted_array = (uint32_t *)array->array;
                casted_array[i] = casted_array[i+1];
                break;
            }
            case 8:{
                uint64_t *casted_array = (uint64_t *)array->array;
                casted_array[i] = casted_array[i+1];
                break;
            }
        } 
    }
    array->length--;
    karray_unlock(array);
}

karray *karray_new(uint8_t elementsz, void(*karray_data_free_fn)(void *)){
    switch(elementsz){
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        default:
            KERROR("0x%x is not a valid element size", elementsz);
            return 0x0;
            break;
    }
    karray *ret = kmalloc(sizeof(karray));
    *ret = (karray){
        .elementsz = elementsz,
        .length = 0,
        .alloc = 64,
        .karray_data_free_fn = karray_data_free_fn,
        .lock = 0
    };
    ret->array = kmalloc(ret->alloc*ret->elementsz);
    return ret;
}
void karray_free(karray *array){
    karray_lock(array);
    if(array->karray_data_free_fn){
        switch (array->elementsz){
            case 1:{
                uint8_t *casted_array = (uint8_t *)array->array;
                for(int i = 0; i < array->length; i++){
                    array->karray_data_free_fn((void*)(uintptr_t)casted_array[i]);
                }
                break;
            }
            case 2:{
                uint16_t *casted_array = (uint16_t *)array->array;
                for(int i = 0; i < array->length; i++){
                    array->karray_data_free_fn((void*)(uintptr_t)casted_array[i]);
                }
                break;
            }
            case 4:{
                uint32_t *casted_array = (uint32_t *)array->array;
                for(int i = 0; i < array->length; i++){
                    array->karray_data_free_fn((void*)(uintptr_t)casted_array[i]);
                }
                break;
            }
            case 8:{
                uint64_t *casted_array = (uint64_t *)array->array;
                for(int i = 0; i < array->length; i++){
                    array->karray_data_free_fn((void*)casted_array[i]);
                }
                break;
            }
        }
    }
    kfree(array->array);
    array->array = 0x0;
    array->length = 0x0;
    array->alloc = 0x0;
    array->elementsz = 0x0;
    kfree(array);
}
void karray_debug_print(karray *array){
    karray_lock(array);
    kprint("{ ");
    switch(array->elementsz){
        case 1:
	          for(int i = 0; i < array->length; i++){
		            kprintf("0x%x ",((uint8_t *)array->array)[i]);
		            if(i+1 < array->length)
			              kputc(',');
	          }
            break;
        case 2:
	          for(int i = 0; i < array->length; i++){
		            kprintf("0x%x ",((uint16_t *)array->array)[i]);
		            if(i+1 < array->length)
			              kputc(',');
	          }
            break;
        case 4:
	          for(int i = 0; i < array->length; i++){
		            kprintf("0x%x ",((uint32_t *)array->array)[i]);
		            if(i+1 < array->length)
			              kputc(',');
	          }
            break;
        case 8:
	          for(int i = 0; i < array->length; i++){
		            kprintf("0x%x ",((uint64_t *)array->array)[i]);
		            if(i+1 < array->length)
			              kputc(',');
	          }
            break;
    }
	kprint("}\n");
    karray_unlock(array);
}
