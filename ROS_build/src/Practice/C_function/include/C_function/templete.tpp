template <typename TL>
void Data::Go(TL &input, TL &output)
{
    TL compare;
    compare.x = input.x - output.y;
    std::cout << "compare x = " << compare.x << std::endl;
}

template <typename TL>
void Data::Turn(TL &input, TL &output)
{
    TL compare;
    compare.x = input.x + output.y;
    std::cout << "compare x = " << compare.x << std::endl;
}
