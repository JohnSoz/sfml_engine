
if openWindow == true then
L_testWindow:addWindow(openWindow)
for i = 1, 50 do 
	local str = string.format("Counter %i \n",i)
   L_testWindow:addText(str)
end
end

