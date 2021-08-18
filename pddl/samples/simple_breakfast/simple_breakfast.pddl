(define (problem simple_breakfast) (:domain robochef)

(:objects
    mug1 - mug
    pan1 - pan
    table1 - table
    plate1 - plate
    egg1 - egg
    cm1 - coffeeMachine
    coffee1 - coffee

    ; loc_self loc_mug loc_pan loc_table loc_plate loc_egg loc_cm - location
)

(:init ;todo: put the initial state's facts and numeric values here
    ;Setting initial locations
    ; (is-at SELF loc_self)
    ; (is-at mug1 loc_egg)
    ; (is-at pan1 loc_pan)
    ; (is-at table1 loc_table)
    ; (is-at plate1 loc_plate)
    ; (is-at egg1 loc_egg)
    ; (is-at cm1 loc_cm)

    ;Everything in the scene is visible
    (is-visible egg1 SELF)
    (is-visible coffee1 SELF)
    (is-visible mug1 SELF)
    (is-visible pan1 SELF)
    (is-visible plate1 SELF)
    (is-visible cm1 SELF)
    (is-visible table1 SELF)
    ;We assume that the exact location of the items is unknown to the robot but he can see them and reach them
    
    ;The robot is initially by the table
    (is-at SELF table1)

    (is-coffeMachine-available cm1)

)

(:goal ;todo: put the goal condition here
     (and (is-cooked egg1)
     (contains plate1 egg1)
     (contains mug1 coffee1)
     (on table1 plate1)
     (on table1 mug1)
     
     )
)

)

;un-comment the following line if metric is needed
;(:metric minimize (???))