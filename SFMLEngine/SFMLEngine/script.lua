
local a = A()
a:getAdr()
a:printName()

local test = Test()
test:setA(a) --set Test::a pointer to a
test:setNewName('newName')
test:getAdr()

a:printName()

test:setNewName('newName#2')

a:printName()