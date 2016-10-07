#ifndef LUA_ADAPTER_H
	#define LUA_ADAPTER_H
	
	#include <iostream>
	#include <lua.hpp>
	#include <lauxlib.h>
	#include <lualib.h>

	typedef int (*Lua_callback_function)(lua_State *L); 

class LuaAdapter {
	
    private:
        lua_State *Lua;
        bool loaded;
        bool print;
        const std::string outputPrefix;
        bool getField(const char* name);
        bool getI(unsigned short int i);
        bool getGlobal(const char* name);               

    public:	

        LuaAdapter();
        ~LuaAdapter();	

        /**
        * Initialize Lua and all its libs
        * Does NOT load any lua-sourcefile
        * Set your default (local) vars after calling this function!         
        */
        void init(); 

        /** 
        * Loads a *.lua-sourcefile         
        * Set your default (local) lua-vars before you call this function!
        * @param name lua-filename
        * @return true on success, false on error
        */
        bool load(const char* name);
        bool load(const std::string name);

        /**
        * @param name Name of the variable inside loaded .lua-file
        * @param result value is saved in this variable 
        * @return true on success, false on error
        */
        bool get(const char* name, int &result);       	
        bool get(const char* name, std::string &result);        
        bool get(const char* name, double &result);
        bool get(const char* name, float &result);

        /**
        * Opens a lua-table
        * @param name Name of the table inside loaded .lua-file         
        * @return true on success, false on error
        */
        bool openTable(const char* name);    
        bool openTable(std::string name){
            return this->openTable(name.c_str());
        }

        /**
        * Opens a table(-field) inside a table
        * @param name Name of the table inside the opened table       
        * @return true on success, false on error
        */
        bool openNestedTable(const char *name);
        bool openNestedTable(std::string name){
            return this->openNestedTable(name.c_str());
        }

        /**
        * Gets a field from an opened table
        * @param name Name of the field
        * @param result value of the field
        * @return true on success, false on error
        */
        bool getField(const char *name, std::string &result);        
        bool getField(const char *name, int &result);

        /**
        * Gets a field from an opened table
        * @param i i-th field (starting at 1)
        * @param result value of the field
        * @return true on success, false on error
        */
        bool getField(unsigned short int i, int &result);
        bool getField(unsigned short int i, double &result);
        bool getField(unsigned short int i, float &result);

        /**
        * Gets a ("2D"-)field value from an opened table.
        * NOTE: DO NOT CALL openNestedTable(name) for this!
        * Example: identity ={
        *               {1, 0, 0},
        *               {0, 1, 0},
        *               {0, 0, 1},                       
        *           }
        * @param j row
        * @param i col
        * @param result value of the field
        * @return true on success, false on error
        */
        bool getNestedField(unsigned short int j, unsigned short int i, double &result);
        bool getNestedField(unsigned short int j, unsigned short int i, float &result);
        bool getNestedField(unsigned short int j, unsigned short int i, int &result);

        /**
        * Sets the value of global lua-var.
        * @param name of the variable
        * @param value the var's value
        * @return true on success, false on error       
        */
        bool set(const char *name, const char *value);        
        bool set(const char *name, int value);
        bool set(const char *name, const double value);
        bool set(const char *name, const float value);

        /**
        * Closes a table
        * Call this function after every opening (and use) of a table!
        * (Prevents "stack-smashing".)         
        */
        void closeTable(){
            return this->pop(1);
        }
        
        /**
        * Calls a lua-function
        * @param name of the function
        * @param argc number of arguments passed to the function
        * @param args arguments
        * @param result returned value of the function
        * @return true on success, false on error       
        */
        bool callFunction(const char* name, const unsigned short int argc, int args[], int &result);

        /**
        *   
        */
        void pushFunction(Lua_callback_function, const char*) ;

        /**
        * Resets Lua's internal stack
        * @return true on success, false on error      
        */
        bool flush();

        /**
        * Set debug output
        * @param mode if mode==true, lua-adapter will print out everything	         
        */
        void setLogMode(bool mode);  

        /**
        * Pops i entries from Lua's internal stack
        * @param i number of entries     
        */
        void pop(signed short int i=1) {
            lua_pop(this->Lua, i);		
        }

        /**
        * Gets the stack position
        * @return the stack position
        */
        int getTop() const {
            return lua_gettop(this->Lua);
        }

        /**
        * Gets the value-type of the current stack position
        * @return the type
        */
        int getType() const {
            return lua_type(this->Lua, 0);
        }

        /** 
        * "Closes" the loaded *.lua-sourcefile
        * This function is called by the destructor of this class!
        * */		
        void close();		
    };

    #endif
