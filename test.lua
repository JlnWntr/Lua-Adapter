number = 33.33
float = 3.141

title = "A title"

width = 600
height = 400
fullscreen = true

matrix ={
    {111,222,333},
    {444,555,666},
    {777,888,999}
}

-- 123 is a default-value for GlobalVar but CAN be changed from C/C++ BEFORE
-- loading the actual lua-file (see test.cpp)
if GlobalVar == nil then GlobalVar = 123 end


Table1 = {ID = 1,   Value = GlobalVar, Text = "Test"}      -- Val = 123

Table2  = {
    t = "Table2",
    X = 1+15-3,
    Y = 1+10,
    W = 3,
    H = 10,
    Test = {
        A= 12, B= 10
    }
}

Strings = {"Key", "Value"};

function gcd(n, m)
    q = math.floor(n/m)
    r = n - m * q
    if r==0 then
        return m
    else
        return gcd(m,r)
    end
end
