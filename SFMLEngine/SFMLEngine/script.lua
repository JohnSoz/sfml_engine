local openWindow = false
local openWindow2 = false

local x = 0; 

a = TestLua('TestLua#2',200,200)
a:addText('text')
a:addWindow(openWindow)

if openWindow == true then
L_testWindow:addWindow(openWindow2)
for i = 1, 10 do 
   local str = string.format("Counter %i \n",i)
   L_testWindow:addText(str)
end
end

