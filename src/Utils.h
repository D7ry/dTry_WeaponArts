#pragma once
namespace Utils
{
	inline void damageav(RE::Actor* a, RE::ActorValue av, float val)
	{
		if (a) {
			a->As<RE::ActorValueOwner>()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, av, -val);
		}
		DEBUG("{}'s {} damaged to {}",
			a->GetName(),
			av,
			a->GetActorValue(av));
	}
}