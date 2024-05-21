#include <iostream>
#include <fstream>
#include <functional>

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

// 1. Удаление элементов от begin до end (включительно)
void Flist::clear(ptrNode begin, ptrNode end)
{
    while (begin != end->next) {
        del_after(begin);
    }
}

// 2. Удаление последнего элемента
void Flist::delete_last()
{
    if (!empty())
    {
        ptrNode ptr = head;
        while (ptr->next != tail) {
            ptr = ptr->next;
        }
        del_by_pointer(ptr->next);
        tail = ptr;
    }
}

// 3. Поиск по условию
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

// 4. Проверка свойств списка
bool Flist::check_properties(std::function<bool(ptrNode)> property)
{
    bool result = property(head->next);
    return result;
}

// 5. Сформировать список из номеров максимальных элементов
void Flist::print_max_elements_indices(std::ostream& stream)
{
    if (!empty())
    {
        ptrNode ptr = head->next;
        Tinfo max_value = ptr->info;
        size_t index = 0, max_index = 0;
        while (ptr) {
            if (ptr->info > max_value) {
                max_value = ptr->info;
                max_index = index;
            }
            ptr = ptr->next;
            ++index;
        }

        stream << "Max element indices: ";
        ptr = head->next;
        index = 0;
        while (ptr) {
            if (ptr->info == max_value) {
                stream << index << ' ';
            }
            ptr = ptr->next;
            ++index;
        }
        stream << '\n';
    }
}

// 6. Поменять местами первые и последние элементы
void Flist::swap_first_and_last()
{
    if (!empty() && head->next != tail)
    {
        ptrNode first = head->next;
        ptrNode ptr = head;
        while (ptr->next != tail) {
            ptr = ptr->next;
        }
        ptr->next = head->next;
        head->next = tail;
        tail->next = first->next;
        first->next = nullptr;
        tail = first;
    }
}

int main()
{
    std::ifstream file("data.txt");
    if (file)
    {
        Flist list;
        ptrNode beg{}, end{};
        list.create_by_queue(file);
        list.print("Initial LIST:");

        // Пример использования функции clear
        if (list.get_size() > 3) {
            ptrNode clear_begin = list.get_head(); // Первый элемент списка
            ptrNode clear_end = clear_begin->next->next; // Третий элемент списка
            list.clear(clear_begin, clear_end);
            list.print("LIST after clear:");
        }

        // Пример использования функции delete_last
        list.delete_last();
        list.print("LIST after deleting last element:");

        // Пример использования функции find_if
        ptrNode found = list.find_if(list.get_head()->next, list.get_tail(), [](Tinfo val) { return val % 2 == 0; });
        if (found)
            std::cout << "First even element found: " << found->info << '\n';
        else
            std::cout << "No even element found.\n";

        // Пример использования функции check_properties для проверки упорядоченности по возрастанию
        bool is_sorted = list.check_properties([](ptrNode ptr) {
            bool sorted = true;
            while (ptr && ptr->next) {
                if (ptr->info > ptr->next->info)
                    sorted = false;
                ptr = ptr->next;
            }
            return sorted;
            });
        std::cout << "List is " << (is_sorted ? "sorted.\n" : "not sorted.\n");

        // Пример использования функции check_properties для проверки арифметической прогрессии
        bool is_arithmetic_progression = list.check_properties([](ptrNode ptr) {
            bool arithmetic = true;
            if (ptr && ptr->next) {
                int diff = ptr->next->info - ptr->info;
                ptr = ptr->next;
                while (ptr && ptr->next) {
                    if (ptr->next->info - ptr->info != diff)
                        arithmetic = false;
                    ptr = ptr->next;
                }
            }
            return arithmetic;
            });
        std::cout << "List is " << (is_arithmetic_progression ? "an arithmetic progression.\n" : "not an arithmetic progression.\n");

        // Пример использования функции print_max_elements_indices
        list.print_max_elements_indices();

        // Пример использования функции swap_first_and_last
        list.swap_first_and_last();
        list.print("LIST after swapping first and last elements:");

        // Повторное добавление элементов для демонстрации удаления последнего элемента
        list.add_to_tail(42);
        list.add_to_tail(99);
        list.print("LIST after adding elements:");
        list.delete_last();
        list.print("LIST after deleting last element:");
    }

    std::cin.get();
    return 0;
}
