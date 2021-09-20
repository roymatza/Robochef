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

class CookEgg(Task):
    def __init__(self) -> None:
        super().__init__(
            name = "serve-egg",
            types = ["egg", "pan","plate", "countertop", "stoveburner"],
            init_predicates=[#"empty plate",
            #"empty pan",
            "not cooked egg",
            "not toggled stoveburner"],
            goal_predicates=["on countertop plate",
            "egg_cracked egg",
            "contains plate egg",
            "cooked egg"])

class SliceTomato(Task):
    def __init__(self) -> None:
        super().__init__(
            name = "make-salad",
            types = ["tomato", "bowl", "countertop"],
            init_predicates=[#"empty bowl"
            ],
            goal_predicates=["sliced tomato",
            "on countertop bowl",
            "contains bowl tomato"])

class SetTable(Task):
    def __init__(self) -> None:
        super().__init__(
            name = "set-table",
            types = ["countertop", "plate", "cup", "fork", "knife", "spoon"],
            init_predicates=[],
            goal_predicates=["on countertop plate",
            "on countertop cup",
            "on countertop fork",
            "on countertop knife",
            "on countertop spoon"])

class ClearDishes(Task):
    def __init__(self) -> None:
        super().__init__(
            name = "clear-dishes",
            types = ["sink", "plate", "cup", "fork", "knife", "spoon"],
            init_predicates=[],
            goal_predicates=["contains sink plate",
            "contains sink cup",
            "contains sink fork",
            "contains sink knife",
            "contains sink spoon"])

class MakeToast(Task):
    def __init__(self) -> None:
        super().__init__(
            name = "make-toast",
            types = ["bread", "toaster", "plate", "countertop"],
            init_predicates=[#"empty plate",
            #"empty toaster",
            "not toggled toaster",
            "not cooked bread"],
            goal_predicates=["on countertop plate",
            "sliced bread",
            "cooked bread",
            "contains plate bread"])