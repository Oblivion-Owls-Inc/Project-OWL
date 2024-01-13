-- This print function is a built in function in Lua
-- This will print to the console and be captured by C++
-- To our custom print function
print("Hello from Main Lua Script!")

-- This is the main table for the main Lua script
-- This is simply an example of how to use the main table
main = {

    -- This is the First function that is in the main table
    [1] = {

        -- This is a Gloable variable that can be accessed from anywhere 
        -- Init becomes the Init function for the main Lua script
        -- This allows C++ to ask for "Init" and it will call this function
        Init = function()
            print("Initializing from Main Lua Script!")
        end
    },

    -- This is a Gloable variable that can be accessed from anywhere 
    -- This is the Second function that is in the main table
    -- This allows C++ to ask for "Update" and it will call this function
    [2] = {
        Update = function()
           -- print("Updating from Main Lua Script!")
        end
    },

    -- This is a Gloable variable that can be accessed from anywhere 
    -- This is the Third function that is in the main table
    -- This allows C++ to ask for "Exit" and it will call this function
    [3] = {
        Exit = function()
            print("Exiting from Main Lua Script!")
        end
    }
}


-- Prints that the Lua script has been loaded successfully
print("Lua script loaded successfully")



--- =========== Functions that can be accessed from Entity ===========
--- Entity() - Creates a new Entity and Returns it
--- IsDestroyed() - Returns true if the Entity is destroyed
--- Destroy() - Destroys the Entity
--- SetName(string) - Sets the name of the Entity
--- GetName() - Returns the name of the Entity
--- AddComponent(string) - Adds a Component to the Entity
--- Clone() - Clones the Entity and Returns a new Entity
--- ===================================================================


--- =========== Functions that can be accessed from Global ============
--- print(Arg) - Prints the Arg to the Debub Console
--- AddToEntitySystem(Entity) - Adds the Entity to the Entity System
--- GetEntityByName(string) - Returns the Entity with the given name
--- ===================================================================