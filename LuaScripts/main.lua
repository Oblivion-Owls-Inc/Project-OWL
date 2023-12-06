-- Main Lua Script for the project
print("Hello from Main Lua Script!")
-- This is the main table for the main Lua script
main = {

    -- This is the First function that is in the main table
    [1] = {
        -- Init becomes the Init function for the main Lua script
        -- This allows C++ to ask for "Init" and it will call this function
        Init = function()
            print("Initializing from Main Lua Script!")
        end
    },
    -- This is the Second function that is in the main table
    -- This allows C++ to ask for "Update" and it will call this function
    [2] = {
        Update = function()
            print("Updating from Main Lua Script!")
        end
    },
    -- This is the Third function that is in the main table
    -- This allows C++ to ask for "Exit" and it will call this function
    [3] = {
        Exit = function()
            print("Exiting from Main Lua Script!")
        end
    }
}

print("Lua script loaded successfully")