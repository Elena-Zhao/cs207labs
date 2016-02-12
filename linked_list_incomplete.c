
//your code here
int get_index(Item* listptr, int value){
    int ctr = 0;
    Item* p;
    for(p = listptr; p!= NULL; p = p->rest){
        if (p->value==value){
            return ctr;
        }
        ctr++;
    }
    return -1;
}

Item* remove_item(Item* listptr, int value){
    int ctr = 0;
    Item* p;
    Item* p_next;
    for(p = listptr; p!= NULL; p = p->rest){
        p_next = p->rest
        if (p_next->value==value){
            p->rest = p_next->rest;
            free(p_next)
        }
    }
    return listptr;
}