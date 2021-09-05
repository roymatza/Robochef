from scene_info import Object

class Task:
    def __init__(self, name, types, init_predicates, goal_predicates) -> None:
        self.name = name
        self.types = types
        self.init_predicates = init_predicates
        self.goal_predicates = goal_predicates

class ServeCoffee(Task):
    def __init__(self) -> None:
        super().__init__(
            name = "serve-coffee",
            types = ["mug", "countertop", "coffeemachine"],
            init_predicates=["not has_coffee mug",
            "not toggled coffeemachine"],
            goal_predicates=["on countertop mug",
            "has_coffee mug"])

class ServeEgg(Task):
    def __init__(self) -> None:
        super().__init__(
            name = "serve-egg",
            types = ["egg", "pan","plate", "countertop", "stoveburner"],
            init_predicates=["empty plate",
            "empty pan",
            "not cooked egg",
            "not toggled stoveburner"],
            goal_predicates=["on countertop plate",
            "egg_cracked egg",
            "contains plate egg",
            "cooked egg"])