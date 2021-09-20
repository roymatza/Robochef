
(define (problem {{data.pname}}) (:domain robochef)

(:objects
    {% for key in data.objects%}
        {{data.objects[key].metadata["name"]}} - {{key}}
    {% endfor %}
)

(:init ;todo: put the initial state's facts and numeric values here
    (near START_LOC)
    {% for init_pred in data.init%}
        ({{init_pred}})
    {% endfor %}
)

(:goal ;todo: put the goal condition here
    (and
        {% for goal_pred in data.goal%}
        ({{goal_pred}})
    {% endfor %})
)

)

;un-comment the following line if metric is needed
;(:metric minimize (???))