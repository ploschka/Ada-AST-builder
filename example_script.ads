procedure print(s: String)
is
begin
    Ada.Text_IO.Put_Line(s);
end print;

function rangeSum(rangeStart: Integer; rangeEnd: Integer) return Integer
is
begin
    s := 0;
    b := 0;
    for i in rangeStart .. rangeEnd loop
        s := s + i;
    end loop;
    return s;
end rangeSum;

procedure main() is begin
    if 1 + 2 = 5 and true or not false then
        print("Helloworld!");
    elsif false then
        print("Godbyeworld?");
    else
        print(rangeSum(100, 1000));
    end if;
end main;

main();