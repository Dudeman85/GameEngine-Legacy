# Entity Component System

Components are aggregate structs or classes, meaning no user defined constructors, and no private or virtual members! They are initialized with a designated initializer {}, just like arrays.
Since components usually should not have any methods, it is good to keep them as structs.