#include <iostream>
#include <fstream>
#include <functional>
//на лабах нужно задачи нужно решать через переключление указателей

//дз: нужно написать void clear(ptrNode begin, ptrNode end) то есть чтобы удалялось всё от определенного до определенного в листе.
//Ещё надо удалить последний элемент 
//написать find_if - поиск по условию сигнатура (ptrNode begin, ptrNode end, условие поиска в виде лямбды), возвращает ptrNode

//инициализая
//1) создание пустого списка
//2) проверка на пустоту
//3) добавление элементов (в голову(в начало)), в хвост
//4) удаление элемента из списка (тоже два варианта)
//создание
//1) по принципу стека
//2) по принципу очереди
//3) упорядоченно
//обработка списка на примере функции печати
//уничтожение списка(деструктор)

using Tinfo = int;
struct NODE
{
    Tinfo info;
    NODE* next;
    NODE(Tinfo info, NODE* ptr = nullptr) : info(info), next(ptr) {};
    ~NODE() { next = nullptr; };
};

using ptrNode = NODE*;

struct Flist {
private:
    ptrNode head, tail;
    size_t size;
    void adding_by_pointer(ptrNode& ptr, Tinfo elem)
    {
        ptr = new NODE(elem, ptr);
        ++size;
    }
    void del_by_pointer(ptrNode& ptr)
    {
        ptrNode p = ptr;
        ptr = p->next;
        delete p;
        --size;
    }
public:
    Flist()
    {
        tail = head = new NODE(0);
        size = 0;
    }
    ~Flist();
    ptrNode get_head()
    {
        return head;
    }
    ptrNode get_tail()
    {
        return tail;
    }
    void set_tail(ptrNode ptr)
    {
        tail = ptr;
    }
    Tinfo get_elem(ptrNode ptr)
    {
        return ptr->info;
    }
    size_t get_size()
    {
        return size;
    }
    bool empty()
    {
        return !head->next;
    }
    void add_to_head(Tinfo elem);
    void add_to_tail(Tinfo elem);
    void add_after(ptrNode ptr, Tinfo elem);
    void del_from_head();
    void del_after(ptrNode ptr);
    void create_by_queue(std::ifstream& file);
    void print(const char* message, std::ostream& stream = std::cout);
    void sorting();
    void remove_odd_elements();
};

bool delete_all_even(Flist& list)
{
    bool result{};
    ptrNode ptr = list.get_head();
    while (!ptr->next)
    {
        if (ptr->next->info % 2 == 0)
        {
            list.del_after(ptr);
            result = true;
        }
        else
            ptr = ptr->next;
    }

    return result;
}

bool double_all_even(Flist& list)
{
    bool result{};
    ptrNode ptr = list.get_head()->next;
    while (ptr)
    {
        if (ptr->info % 2 == 0)
        {
            list.add_after(ptr, ptr->info);
            result = true;
            ptr = ptr->next->next;
        }
        else
            ptr = ptr->next;
    }
    return result;
}

bool find_fragment_list_1(Flist& list, ptrNode& beg, ptrNode& end)
{
    auto one_parity = [](int x, int y)
        {
            return (x + y) % 2 == 0; 
        };
    beg = end = nullptr;
    ptrNode head = list.get_head();
    ptrNode ptr = head;
    while (ptr->next)
    {
        if ((ptr == head || !one_parity(ptr->info, ptr->next->info)) && ptr->next->next && one_parity(ptr->next->info, ptr->next->next->info))
            beg = ptr;
        else if (one_parity(ptr->info, ptr->next->info) && (!ptr->next->next || (ptr->next->next && !one_parity(ptr->next->info, ptr->next->next->info))))
            end = ptr->next; 
        ptr = ptr->next; 
    }
    if (beg && end)
        std::cout << beg->info << ' ' << end->info << '\n';
    return end != nullptr;
}

bool find_fragment_list_2(Flist& list, ptrNode& beg, ptrNode& end)
{
    auto one_parity = [](int x, int y)
        {
            return (x + y) % 2 == 0;
        };
    beg = end = nullptr;
    ptrNode head = list.get_head();
    ptrNode ptr = head;
    bool isBeg{};
    while (ptr->next)
    {
        if (ptr->next->next && one_parity(ptr->next->info, ptr->next->next->info))
        {
            if (!isBeg)
            {
                beg = ptr;
                isBeg = true;
            }

        }
        else if (one_parity(ptr->info, ptr->next->info))
        {
            end = ptr->next;
            isBeg = false;
        }
        ptr = ptr->next;
    }
    if (beg && end)
        std::cout << beg->info << ' ' << end->info << '\n';
    return end != nullptr;
}

bool task(Flist& list)
{
    ptrNode beg{}, end{};
    bool res = find_fragment_list_1(list, beg, end);
    if (res && end->next)
    {
        ptrNode tail = list.get_tail();
        ptrNode tmp = beg->next;
        beg->next = end->next;
        end->next = tail;
        tail->next = tmp;
        list.set_tail(end);
    }

    ptrNode head = list.get_head();
    if (beg != head)
    {
        ptrNode tmp = end->next;
        end->next = head->next;
        head->next = beg->next;
        beg->next = tmp;
        if (end == list.get_tail())
            list.set_tail(beg);
    }

    return res;
}

int main()
{

    std::ifstream file("data.txt");
    if (file)
    {
        Flist list;
        ptrNode beg{}, end{};
        list.create_by_queue(file);
        list.print("LIST:");
        std::cout << list.get_size() << '\n';
        std::cout << list.get_tail()->info << '\n';
        list.sorting();
        list.print("LIST Ordered:");
        if (find_fragment_list_1(list, beg, end))
            std::cout << "NO\n";
        std::cout << list.get_tail()->info << '\n';


    }

    std::cin.get();
    return 0;
}

Flist::~Flist()
{
    while (!empty())
        del_from_head();
    delete head;
    //head = tail = nullptr;
}

void Flist::add_to_head(Tinfo elem)
{
    adding_by_pointer(head->next, elem);
    if (head == tail)
        tail = tail->next;
}

void Flist::add_to_tail(Tinfo elem)
{
    adding_by_pointer(tail->next, elem);
    tail = tail->next;
}

void Flist::add_after(ptrNode ptr, Tinfo elem)
{
    if (ptr)
    {
        adding_by_pointer(ptr->next, elem);
        if (ptr == tail)
            tail = tail->next;
    }
}

void Flist::del_from_head()
{
    if (head->next)
    {
        del_by_pointer(head->next);
        if (!head->next)
            tail = head;

    }
}

void Flist::del_after(ptrNode ptr)
{
    if (ptr && ptr->next)
    {
        if (ptr->next == tail)
            tail = ptr;
        del_by_pointer(ptr->next);
    }
}

void Flist::create_by_queue(std::ifstream& file)
{
    Tinfo elem;
    while (file >> elem)
        add_to_tail(elem);

}

void Flist::print(const char* message, std::ostream& stream)
{
    stream << message << '\n';
    ptrNode ptr = head->next;
    while (ptr)
    {
        stream << ptr->info << ' ';
        ptr = ptr->next;
    }
    stream << '\n';
}

void Flist::sorting()
{
    auto switch_pointers = [](ptrNode q, ptrNode p)
        {
            ptrNode tmp = p->next;
            p->next = tmp->next;
            tmp->next = q->next;
            q->next = tmp;
        };
    auto find_place = [this](Tinfo elem)->ptrNode
        {
            ptrNode result = head;
            while (result->next && result->next->info < elem)
                result = result->next;
            return result;
        };
    ptrNode hprev = head->next, h = head->next->next;
    while (h)
    {
        if (hprev->info > h->info)
        {
            switch_pointers(find_place(h->info), hprev);
            h = hprev->next;
        }
        else
        {
            hprev = h;
            h = h->next;
        }
        tail = hprev;
    }
}


