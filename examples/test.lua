number = 33.33
float = 3.141

title = "A title"
--~ if _VERSION == 'Lua 5.4' then--test this 'const-feature ':
    --~ local test_const_title<const> = "A const title"
    --~ title = test_const_title
--~ end

width = 600
height = 400
fullscreen = true

matrix ={
    {111,222,333},
    {444,555,666},
    {777,888,999}
}

Table1 = {ID = 1,   Value = 123, Text = "Test"}      -- Val = 123

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

function Print(t)
    print("Lua: " .. tostring(t))
end

function Sum3(a, b, c)
    return a + b + c
end

function Random()
    return 5
end

function Inc(x)
    return x+1
end

function String()
    return "This is a string!"
end


-- Call C/C++-function
if not (test_function==nil)  then
   print ("Lua: ".. 44 .. " * 2 = " .. test_function(44) .. " (C-function 'test_function' was called.)")
end