local openWindow = false
local openWindow2 = false


a = A('TestLua#2',200,200)--
a:addText('asd')
a:addWindow(openWindow)

if openWindow == true then
L_testWindow:addWindow(openWindow2)
for i = 1, 5 do 
   local str = string.format("Counter %i \n",i)
   L_testWindow:addText(str)
end
end

