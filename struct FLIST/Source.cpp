#include <iostream>
#include <fstream>
#include <functional>
#include <string>

using namespace std;
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

// Поиск по условию
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

bool remove_every_second_multiple_of_5(Flist& list) {
    ptrNode ptr = list.get_head();
    int count = 0;
    bool res{ 0 };

    while (ptr && ptr->next) {
        if (ptr->next->info % 5 == 0) {
            count++;
            if (count % 2 == 0) {
                list.del_after(ptr);
                res = true;
            }
            else {
                ptr = ptr->next;
            }
        }
        else {
            ptr = ptr->next; // указатель двигаем если не кратно  5
        }
    }
    return res;
}


// Функция проверки наличия чисел, кратных 5, в списке.
//bool has_multiples_of_5(Flist& list) {
//    bool mult_5{ 0 };
//    ptrNode current = list.get_head()->next;
//    while (current) {
//        if (current->info % 5 == 0) {
//            mult_5 = true;
//        }
//        current = current->next;
//    }
//    return mult_5;
//}

int main()
{
    ifstream input_file("input.txt");
    if (!input_file) {
        cout << "Failed to open input file" << endl;
    }

    Flist list;
    list.create_by_queue(input_file);
    list.sorting();

   

    ofstream output_file("output.txt");
    if (output_file) {
        if (remove_every_second_multiple_of_5(list))
            list.print("Modified sequence:", output_file);
        else {
            output_file << "There are no numbers divisible by 5 in the sequence." << endl;
        }
    }
    else {
        output_file << "file error" << endl;
    }

    

    return 0;
}
