import json
import math
import os
import random


ARENA_HALF_SIZE = 1.85
RESOURCE_COUNT = 15
RESOURCE_RADIUS = 0.18
RESOURCE_QUANTITY = 50
MIN_RESOURCE_DISTANCE = 0.45
STATE_FILE = os.path.join(
    os.environ.get("TMPDIR", "/tmp"),
    "argos_python_foraging_state_{}.json".format(os.getpid()),
)

resources = []


def _resource_dict(resource_id, x, y):
    return {
        "id": resource_id,
        "x": round(x, 3),
        "y": round(y, 3),
        "radius": RESOURCE_RADIUS,
        "quantity": RESOURCE_QUANTITY,
        "max_quantity": RESOURCE_QUANTITY,
    }


def _distance_xy(a, b):
    return math.hypot(a["x"] - b["x"], a["y"] - b["y"])


def _new_position(resource_id):
    for _ in range(200):
        candidate = _resource_dict(
            resource_id,
            random.uniform(-ARENA_HALF_SIZE, ARENA_HALF_SIZE),
            random.uniform(-ARENA_HALF_SIZE, ARENA_HALF_SIZE),
        )
        if all(_distance_xy(candidate, other) >= MIN_RESOURCE_DISTANCE for other in resources):
            return candidate
    return _resource_dict(resource_id, 0.0, 0.0)


def _generate_resources():
    global resources

    random.seed(17)
    resources = []
    for resource_id in range(RESOURCE_COUNT):
        resources.append(_new_position(resource_id))
    _write_state()


def _write_state():
    state = {"resources": resources}
    tmp_file = STATE_FILE + ".tmp"
    with open(tmp_file, "w", encoding="utf-8") as state_file:
        json.dump(state, state_file)
    os.replace(tmp_file, STATE_FILE)


def _robot_position(robot):
    position = robot.position.get_position()
    return float(position[0]), float(position[1])


def _resource_at(x, y):
    for resource in resources:
        if math.hypot(x - resource["x"], y - resource["y"]) <= resource["radius"]:
            return resource
    return None


def _set(robot, key, value):
    robot.variables.set_attribute(key, str(value))


def _reset_robot(robot):
    _set(robot, "harvest", "0")
    _set(robot, "resource_id", "")
    _set(robot, "resource_quantity", "0")
    _set(robot, "carrying", "0")
    _set(robot, "score", robot.variables.get_attribute("score") or "0")


def _respawn(resource):
    resource.update(_new_position(resource["id"]))


def _harvest(robot, resource):
    resource["quantity"] -= 1
    _set(robot, "carrying", "1")
    _set(robot, "resource_id", resource["id"])
    _set(robot, "resource_quantity", max(resource["quantity"], 0))
    _set(robot, "score", int(robot.variables.get_attribute("score") or 0) + 1)

    if resource["quantity"] <= 0:
        _respawn(resource)
        _set(robot, "carrying", "0")
        _set(robot, "resource_id", "")
        _set(robot, "resource_quantity", "0")
        _set(robot, "resource_here", "")
        return True

    return False


def init():
    _generate_resources()
    for robot in allrobots:
        _reset_robot(robot)


def reset():
    _generate_resources()
    for robot in allrobots:
        _reset_robot(robot)


def pre_step():
    changed = False

    for robot in allrobots:
        x, y = _robot_position(robot)
        resource = _resource_at(x, y)

        if resource is None:
            _set(robot, "resource_id", "")
            _set(robot, "resource_quantity", "0")
            _set(robot, "resource_here", "")
            _set(robot, "harvest", "0")
            continue

        _set(robot, "resource_id", resource["id"])
        _set(robot, "resource_quantity", resource["quantity"])
        _set(robot, "resource_here", json.dumps(resource))

        if robot.variables.get_attribute("harvest") == "1" and resource["quantity"] > 0:
            if _harvest(robot, resource):
                _set(robot, "harvest", "0")
                changed = True
                continue
            changed = True

        _set(robot, "harvest", "0")

    if changed:
        _write_state()


def post_step():
    _write_state()


def is_experiment_finished():
    return False


def post_experiment():
    pass


def destroy():
    pass
