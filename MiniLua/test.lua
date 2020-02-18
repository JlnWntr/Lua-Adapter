number = math.pi * 100
float = math.pi
title = "A title"
width = 600
fullscreen = true

-- Call C/C++-function
if not (test_function==nil)  then
   print ("Lua: ".. 44 .. " * 2 = " .. test_function(44) .. " (C-function 'test_function' was called.)")
end


function Sum(a, b, c)
    return a + b + c
end

function Print_string()
    print("Lua: This is a string!")
end

function Power(x)
    print(x*x)
end
