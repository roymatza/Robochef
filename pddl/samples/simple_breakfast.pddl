(define (problem simple_breakfast) (:domain robochef)

(:objects
    mug1 - mug
    pan1 - pan
    table1 - table
    plate1 - plate
    egg1 - egg
    cm1 - coffeeMachine

    ; loc_self loc_mug loc_pan loc_table loc_plate loc_egg loc_cm - location
)

(:init ;todo: put the initial state's facts and numeric values here

    ;Everything in the scene is visible
    (near table1)
    ;We assume that the exact location of the items is unknown to the robot but he can see them and reach them
    (available cm1)

)

(:goal ;todo: put the goal condition here
     (and 
         (cooked egg1)
         (contains plate1 egg1)
         (has-coffee mug1)
         (on table1 plate1)
         (on table1 mug1)
     )
)

)

;un-comment the following line if metric is needed
;(:metric minimize (???))