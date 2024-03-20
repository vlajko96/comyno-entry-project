#include "test_utils.hpp"

extern t_module_t cm_qpid_module;
extern t_module_t cm_pgclient_module;

t_module_t *test_modules[] =
{
    &cm_qpid_module,
    &cm_pgclient_module,
};

void dump_modules(void)
{
    int i;

    std::cout << std::endl;
    for(i=0; i<T_ARRAY_SIZE(test_modules); i++)
    {
        std::cout << i << "\t" << test_modules[i]->name << std::endl;
    }
    std::cout << "-1\texit" << std::endl;
}

void dump_items(t_module_t *m)
{
    int i;

    std::cout << std::endl;
    std::cout << "The current module is: " << std::endl;

    for(i=0; i<m->item_len; i++)
    {
        std::cout << i << "\t" << m->item_list[i].name << std::endl;
    }
    std::cout << "-1\texit" << std::endl;
}

void enter_modules(t_module_t *m)
{
    int ret;
    int idx;

    while(1)
    {
        dump_items(m);
        std::cout << "Please enter your choice: ";
        ret = t_get_int(&idx);
        std::cout << std::endl;
        if(ret < 0)
        {
            std::cout << "Invalid input" << std::endl;
            continue;
        }

        if(idx == -1)
        {
            break;
        }

        if(idx<0 || idx>=m->item_len)
        {
            std::cout << "Not supported idx: " << idx << std::endl;
            continue;
        }

        std::cout << "->Item : " << m->item_list[idx].name << std::endl;
        m->item_list[idx].handle();
    }

}

int main()
{
    int ret;
    int idx;

    while(1)
    {
        dump_modules();
        std::cout << "Please enter your choice: ";
        ret = t_get_int(&idx);
        std::cout << std::endl;
        if(ret < 0)
        {
            std::cout << "Invalid input" << std::endl;
            continue;
        }

        if(idx == -1)
        {
            break;
        }

        if(idx<0 || idx>=T_ARRAY_SIZE(test_modules))
        {
            std::cout << "Not supported idx: " << idx << std::endl;
            continue;
        }

        enter_modules(test_modules[idx]);
    }

    return 0;
}

