
(define (problem problem_407658) (:domain robochef)

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
        (interactable pan1)
        (interactable stoveburner1)
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

;un-comment the following line if metric is needed
;(:metric minimize (???))