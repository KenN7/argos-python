import json
import os


STATE_FILE = os.path.join(
    os.environ.get("TMPDIR", "/tmp"),
    "argos_python_foraging_state_{}.json".format(os.getpid()),
)


def init():
    pass


def _load_resources():
    if not os.path.exists(STATE_FILE):
        return []

    with open(STATE_FILE, "r", encoding="utf-8") as state_file:
        return json.load(state_file).get("resources", [])


def _draw_resource(resource):
    x = float(resource["x"])
    y = float(resource["y"])
    radius = float(resource["radius"])
    quantity = max(0, int(resource["quantity"]))
    max_quantity = max(1, int(resource["max_quantity"]))
    fill_radius = radius * (quantity / max_quantity)

    environment.qt_draw.circle([x, y, 0.002], [], radius, "green", False)
    environment.qt_draw.circle([x, y, 0.003], [], fill_radius, "yellow", True)


def draw_in_world():
    environment.qt_draw.circle([0, 0, 0.001], [], 0.28, "blue", False)

    for resource in _load_resources():
        _draw_resource(resource)


def draw_in_robot():
    if robot.variables.get_attribute("carrying") == "1":
        environment.qt_draw.cylinder([0, 0, 0.065], [], 0.025, 0.035, "yellow")

    if robot.variables.get_attribute("resource_here"):
        environment.qt_draw.circle([0, 0, 0.012], [], 0.055, "green", False)


def destroy():
    pass
