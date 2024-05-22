#include <iostream>
#include <fstream>
#include <functional>
#include <string>

using Tinfo = int;

struct NODE
{
    Tinfo info;
    NODE* next;
    NODE(Tinfo info, NODE* ptr = nullptr) : info(info), next(ptr) {}
    ~NODE() { next = nullptr; }
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
    ptrNode get_head() { return head; }
    ptrNode get_tail() { return tail; }
    void set_tail(ptrNode ptr) { tail = ptr; }
    Tinfo get_elem(ptrNode ptr) { return ptr->info; }
    size_t get_size() { return size; }
    bool empty() { return !head->next; }
    void add_to_head(Tinfo elem);
    void add_to_tail(Tinfo elem);
    void add_after(ptrNode ptr, Tinfo elem);
    void del_from_head();
    void del_after(ptrNode ptr);
    void create_by_queue(std::ifstream& file);
    void print(const char* message, std::ostream& stream = std::cout);
    void sorting();
    void remove_odd_elements();

    void clear(ptrNode begin, ptrNode end);
    void delete_last();
    ptrNode find_if(ptrNode begin, ptrNode end, std::function<bool(Tinfo)> condition);
    bool check_properties(std::function<bool(ptrNode)> property);
    void print_max_elements_indices(std::ostream& stream = std::cout);
    void swap_first_and_last();
};

Flist::~Flist()
{
    while (!empty())
        del_from_head();
    delete head;
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



//Поиск по условию
ptrNode Flist::find_if(ptrNode begin, ptrNode end, std::function<bool(Tinfo)> condition)
{
    ptrNode result = nullptr;
    while (begin != end && !result) {
        if (condition(begin->info)) {
            result = begin;
        }
        begin = begin->next;
    }
    return result;
}


//void create_by_queue_1(ptrNode& ptr, std::ifstream& file) {
//    int x{};
//    if (file >> x)
//    {
//        ptr = new NODE(x);
//        create_by_queue_1(ptr->next, file);
//    }
//}
//
//ptrNode create_by_queue_2(std::ifstream& file) {
//    ptrNode ptr{};
//    int x{};
//    if (file >> x)
//        ptr = new NODE(x, create_by_queue_2(file));
//    return ptr;
//}
//
//void insert_by_order(ptrNode& ptr, int x)
//{
//    if (!ptr || x < ptr->info)
//        ptr = new NODE(x, ptr);
//    else
//        insert_by_order(ptr->next, x);
//
//}
//void create_by_order(ptrNode& ptr, std::ifstream& file)
//{
//    int x{};
//    if (file >> x)
//    {
//        insert_by_order(ptr, x);
//        create_by_order(ptr, file);
//    }
//}
//
//
//void clear(ptrNode& ptr) {
//    if (ptr)
//    {
//        clear(ptr->next);
//        delete ptr;
//        ptr = nullptr;
//    }
//}
//
//
//void print(const ptrNode& ptr)
//{
//    if (!ptr)
//        std::cout << "\n";
//    else
//    {
//        std::cout << ptr->info << " ";
//        print(ptr->next);
//    }
//}
//
//std::string print_to_string(ptrNode ptr)
//{
//    std::string result{};
//    if (ptr)
//        result = std::to_string(ptr->info) + ' ' + print_to_string(ptr->next);
//    return result;
//}
//
//int summa(ptrNode ptr)
//{
//    int sum{};
//    if (ptr)
//        sum = ptr->info + summa(ptr->next);
//    return sum;
//}
//
//int count_even(ptrNode ptr)
//{
//    int cnt{};
//    if (ptr)
//        cnt = (ptr->info % 2 == 0 ? 1 : 0) + count_even(ptr->next);
//    return cnt;
//}
//
//
//ptrNode first_even(ptrNode ptr)
//{
//    ptrNode res{};
//    if (ptr)
//        res = ptr->info % 2 == 0 ? ptr : first_even(ptr->next);
//    return res;
//}
//
//ptrNode last_even(ptrNode ptr) //написать эту таску, но на выходе из рекурсии
//{
//    ptrNode res{};
//    if (ptr)
//    {
//        if (ptr->info % 2 == 0) res = ptr;
//        res = last_even(ptr->next);
//    }
//    return res;
//}
//
//void delete_all_even_1(ptrNode& ptr)
//{
//    if (ptr)
//    {
//        if (ptr->info % 2 == 0) { delete_from_head(ptr); delete_all_even_1(ptr); }
//        else
//            delete_all_even_1(ptr->next);
//    }
//}
//
//void delete_all_even_2(ptrNode& ptr)
//{
//    if (ptr) { delete_all_even_2(ptr->next); if (ptr->info % 2 == 0) delete_from_head(ptr); }
//}
//
//ptrNode copy(ptrNode ptr)
//{
//    ptrNode res{};
//    if (ptr) res = new NODE(ptr->info, copy(ptr->next));
//    return res;
//}


void print_rec( ptrNode ptr)
{
    if (!ptr)
        std::cout << "\n";
    else
    {
        std::cout << ptr->info << " ";
        print_rec(ptr->next);
    }
}

void print(Flist& list)
{
    print_rec(list.get_head()->next);
}

void double_first(Flist& list, ptrNode ptr)
{
    if (ptr) { if (ptr->info % 3 == 0) list.add_after(ptr, ptr->info); else double_first(list, ptr->next); }
}

void  delete_all_even(Flist& list)
{
    ptrNode head = list.get_head();
    std::function<void(ptrNode&)> delete_all_even = [&delete_all_even, &list](ptrNode& head) {
        if (head->next) { delete_all_even(head->next); if (head->next->info % 2 == 0) list.del_after(head); }};
    delete_all_even(head);
}


int main()
{
    std::ifstream file("data.txt");
    ptrNode head = new NODE( 0, create_by_queue_2(file));
    std::cout << print_to_string(head->next)<< '\n';
    clear(head);

    std::cin.get();
    return 0;
}
