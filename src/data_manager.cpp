#include "globals.h"

////////////////////////////////////////////////////////
// Class definition (put this code into data_manager.cpp)
////////////////////////////////////////////////////////

// Insert target_data into the set
void WATER_DATA_MANAGER::insert_data(WATER_DATA::target_data& data)
{
    water_data_set.insert(data);
}

// Remove data from the set based on ID
void WATER_DATA_MANAGER::delete_data(int id)
{

    // Find the element
    auto iter = std::find_if(water_data_set.begin(), water_data_set.end(),
        [id](const WATER_DATA::target_data& d) { return d.id == id; });

    // Erase it if we found a match
    if (iter != water_data_set.end()) {
        water_data_set.erase(iter);
    }
}

// Prints the sorted data
void WATER_DATA_MANAGER::print_all_data()
{
    std::cout << "WATER_DATA:\n";
    for (auto data : water_data_set) {

        debug("ID: ");
        debug(data.id);

        debug(", Name: ");
        debug(data.name);

        debug(", HP: ");
        debug(data.hp);
        debug(", HF: ");
        debug(data.hf);

        debug(", VP: ");
        debug(data.vp);
        debug(", VF: ");
        debug(data.vf);

        debug(", SP: ");
        debug(data.sp);
        debug(", SF: ");
        debug(data.sf);

        debug(", RWT: ");
        debug(data.rwt);
        
        debug(", WATER: ");
        debug(data.water);
        debugln("");
    }
    debugln("");
}
