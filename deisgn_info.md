## MVC

stlviz loosely follows a Model View Controller (MVC) design pattern.

### Model

vcore::model stores all model objects

vobj classes represent model objects storing the state of all visualized elements across all operations.

### View

vcore::view handles the runtime window.

vobj classes also contain their display logic, so that the model data can be easily accessed (violates MVC but is much more convenient).

### Controller

vstd classes wrap std classes to allow the user's code to communicate with the visualizer while retaining original std class logic. Each vstd object has a backing vobj object which they communicate with.

vcore::controller handles the runtime window's input events, as well as blocking logic.
