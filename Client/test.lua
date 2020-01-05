local obj = MyObject(42)
print(obj:get())    -- 42
obj:set(-1.5)
print(obj:get())    -- -1.5

function teste()
    print(obj:get())
end