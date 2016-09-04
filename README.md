# Linenoise Lua Bindings

A simple Lua binding for Linenoise.

```lua
  local ln = require ('linenoise')
```

That's it!

# The API:

* `linenoise.read(prompt)` - Get input from the user.
  * Returns the string on success or `nil` on failure.
* `linenoise.clear()` - Clear the screen.
* `linenoise.historyAdd(line)` - Adds a line of text to linenoise's history buffer.
* `linenoise.historyLoad(filename)` - Loads history from a file.
  * Returns `true` on success or `false` on failure.
* `linenoise.historySave(filename)` - Saves the history buffer to a file.
  * Returns `true` on success or `false` on failure.
* `linenoise.setMaxHistoryLen(len)` - Sets the maximum length of the history buffer,
  culling older entries first if the buffer shrinks.
  * Returns `true` on success or `false` on failure.
* `linenoise.setCompletionCallback(func)` - Sets the callback for when the user presses
  the `Tab` key
* `linenoise.setHintCallback(func)` - Sets the input hinting callback.
* `linenoise.setMultiline(toggle)` - Sets whether linenoise uses multi- or single-line
  mode.
* `linenoise.printKeyCodes()` - Enter debug print mode. Linenoise will only print key
  codes until 'quit' is typed.

# Callbacks:

Linenoise supports two callbacks: **completion**, and **hint**.  These are functions
that linenoise calls to get context-appropriate information to display to the user.

### Completions:
The completion callback function is called every time the user presses `Tab` when not already cycling through a completion list.  This function defines the various options the user has to complete what is being typed.

The completion callback takes two arguments:
* `line` - The current line the user is typing.
* `completions` - A userdata containing the list of completions.

The callback registers completions by calling the `add` method on the `completions` userdata passed as the second argument to the function.

The add method is defined as follows:
* `completions:add(line)` - Adds a possible completion to the list of completions.

The completion callback does not return any meaningful values.

### Hints:
The hint callback function is called every time the line is updated.  Hints are used to provide a 'preview' of what the user could be typing.  Hints cannot be used as completions, however.

The hint callback takes one argument:
* `line` - The current line the user is typing.

The callback may return 0 to 3 values:
* `hint` - The hint string.  Applied to the end of what the user is typing.
* `color` - An integer color for the hint.  As defined by the linenoise README.
* `bold` - A boolean value denoting whether the hint text should be bold.
