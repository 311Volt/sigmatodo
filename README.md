# sigmatodo

None of the todo apps I found on the internet were remotely close to what I wanted,
so I made this.

Usage should feel somewhat familiar to Jira users. However, unlike Jira,
this app is stupidly simple by design.
Tasks are organized into projects and each one of them has freely assignable
"status" and "due date" parameters, which affect placement and appearance on
the task list. That is all, the rest is implementation details.

The primary design goal is speed, since page load times are a major annoyance
with most well-known project management tools. The server should:

- keep response times below 10 ms (it's usually well below 1 ms in practice)
- run on a low-power device such as the Raspberry Pi
- use little memory and CPU so that it may be run in the background without much consideration

In order to accomplish all this simultaneously and keep total page load time
as low as possible, the application is extremely minimal, with C++20, Crow and SQLite at the
backend and plain HTML/CSS at the frontend. No JavaScript is required on the client side.

# requirements

## backend machine

your grandma's pentium 133 from the basement will probably do

## browser

As of now, the UI has only been tested on the most recent versions of Chrome & Firefox
and is not guaranteed to work on other browsers. It is best viewed on Firefox, as it seems
more efficient at rendering simpler pages.

In theory, it should be easily portable to even decades-old browsers.
Support for a number of obsolete/exotic browsers is planned for the future.


# limitations

Since this project exists specifically to cover my personal use case,
it is missing many features that others could consider absolutely essential for
a to-do app.

In particular, there is no multi-user support or access control.
The app is designed to run on a local network (or through a VPN like ZeroTier One)
and used by one person, perhaps more if they trust each other enough.

Other missing things include:

- recurring tasks
- task queries
- prioritizing tasks (other than "in progress"/"on hold" statuses)
- subtasks

These are planned for a future improved version of the app, `ligmatodo`.