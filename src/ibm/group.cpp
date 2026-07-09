#include "group.hpp"

// group creation constructor
Group::Group(
        unsigned const nmembers, 
        Parameters const &params) :
    members(nmembers, Individual(params))
{}

// copy constructor
Group::Group(Group const &other) :
    members(other.members),
    resources(other.resources),
    group_is_dead{other.group_is_dead}
{}

void Group::operator=(Group const &other)
{
    members = other.members;
    resources = other.resources;
    group_is_dead = other.group_is_dead;
}
