(define (problem problem_durative) (:domain robochef_durative)

(:objects
        bread1 - bread
        toaster1 - toaster
        plate1 - plate
        countertop1 - countertop
        egg1 - egg
        pan1 - pan
        stoveburner1 - stoveburner
)

(:init ;todo: put the initial state's facts and numeric values here
    (near START_LOC)
)

(:goal ;todo: put the goal condition here
    (and
        (on countertop1 plate1)
        (sliced bread1)
        (cooked bread1)
        (contains plate1 bread1)
        (on countertop1 plate1)
        (egg-cracked egg1)
        (contains plate1 egg1)
        (cooked egg1)
)
)

)