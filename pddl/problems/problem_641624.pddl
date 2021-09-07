
(define (problem problem_641624) (:domain robochef)

(:objects
        mug1 - mug
        countertop1 - countertop
        coffeemachine1 - coffeemachine
        egg1 - egg
        pan1 - pan
        plate1 - plate
        stoveburner1 - stoveburner
)

(:init ;todo: put the initial state's facts and numeric values here
        (empty plate1)
        (empty pan1)
        (interactable countertop1)
        (near coffeemachine1)
)

(:goal ;todo: put the goal condition here
    (and
        (on countertop1 mug1)
        (has-coffee mug1)
        (on countertop1 plate1)
        (egg-cracked egg1)
        (contains plate1 egg1)
        (cooked egg1)
)
)

)

;un-comment the following line if metric is needed
;(:metric minimize (???))