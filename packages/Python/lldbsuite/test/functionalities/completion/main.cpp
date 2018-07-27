class Foo
{
public:
    int Bar(int x, int y)
    {
        return x + y;
    }
};

struct Container { int MemberVar; };

int main()
{
    Foo fooo;
    fooo.Bar(1, 2);
    Container container;
    return container.MemberVar = 3; // Break here
}
