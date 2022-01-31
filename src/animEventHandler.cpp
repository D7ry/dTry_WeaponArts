#include "events.h"
#include "data.h"

//all credits to Bingle

RE::BSEventNotifyControl animEventHandler::HookedProcessEvent(RE::BSAnimationGraphEvent& a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* src) {

    FnProcessEvent fn = fnHash.at(*(uint64_t*)this);
    if (a_event.tag != NULL && a_event.holder != NULL) {
        //DEBUG("processing weapon arts from the set");
        //DEBUG("there are currently {} obj from the set", weaponArts::weaponArtsMap.size());
        DEBUG("anim event: {}", static_cast<std::string>(a_event.tag));
        auto it = weaponArts::weaponArtsMap.find(static_cast<std::string>(a_event.tag));
        if (it != weaponArts::weaponArtsMap.end()) {
            std::vector<waObj>& arr = it->second;
            for (auto weaponArtObj = arr.begin(); weaponArtObj != arr.end(); ++weaponArtObj) {
                weaponArtObj->processWeaponArt(a_event.holder->As<RE::Actor>());
            }
        }
    }
    return fn ? (this->*fn)(a_event, src) : RE::BSEventNotifyControl::kContinue;
}

std::unordered_map<uint64_t, animEventHandler::FnProcessEvent> animEventHandler::fnHash;

