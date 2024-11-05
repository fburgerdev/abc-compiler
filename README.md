# abc-compiler
This repo is a compiler for the `abc` language, a c++ like language created by me.

Both the language and the compiler were created for fun, as well as to learn about designing programming languages and building compilers.

## The abc-language

- [Types](spec/Types.md)
- [Generics](spec/Generics.md)
- [Definitions](spec/Definitions.md)
- [Statements](spec/Statements.md)
- [Expressions](spec/Expressions.md)
- [Memory Management](spec/Memory%20Management.md)
- [Built-In](spec/Built-In.md)

### Purpose
The abc language serves an educational purpose. The design and development should teach me, and potentially others, about programming languages and their design as well as about building compilers.

### Philosophy
The abc language is designed and intended to be used similarly to C/C++.

The core principles, in no particular order, are the following:
- Aesthetics - The source code should be visually appealing.
- Performance -  The program should have performance similar to C/C++ programs.
- Simplicity - The languages ruleset should be concise and easy to understand. It‘s syntax should be expressive.

### Example
This a snake game programmed in abc:
```
group Math {
    class Vec2 {
        init(x: int, y: int)
          : x{x}, y{y}

        @static
        func Origin() -> Vec2 {
            return Vec2(0, 0);
        }
        @static
        func Up() -> Vec2 {
            return Vec2(+0, +1);
        }
        @static
        func Right() -> Vec2 {
            return Vec2(+1, +0);
        }
        @static
        func Down() -> Vec2 {
            return Vec2(+0, -1);
        }
        @static
        func Left() -> Vec2 {
            return Vec2(-1, +0);
        }

        op+(other: const &Vec2) const -> Vec2 {
            return Vec2(_x + other.x, _y + other.y);
        }
        op==(other: const &Vec2) const -> Vec2 {
            return _x == other.x and _y == other.y;
        }
        op<(other: const &Vec2) const -> Vec2 {
            return _x < other.x and _y < other.y;
        }
        op<=(other: const &Vec2) const -> Vec2 {
            return _x <= other.x and _y <= other.y;
        }

        var x: int = 0;
        var y: int = 0;
    }
}
group Snake {
    @global
    var DefaultWidth :const int= 20;
    @global
    var DefaultHeight :const int= 20;

    class Game {
        init(size: Math::Vec2)
          : snake{0, 0}, size{size} {
            _spawnApple();
        }

        func move(dir: Math::Vec2) {
            let newpos := _snake.back() + dir;
            if Math::Vec2::Origin() <= newpos and newpos < _size {
                _snake.push_back(newpos);
                _snake.pop_front();
                if (newpos == _apple) {
                    _score++;
                    _spawnApple();
                }
            }
            else {
                _inGame = false;
            }
        }
        func spawnApple() {
            _apple = Vec2(std::random(0, _size.x), std::random(0, _size.y));
            for coords in _snake {
                if _apple == coords {
                    return _spawnApple();
                }
            }
        }

        var inGame: bool = true;
        var score: uint = 0;
        var snake: std::deque<Vec2>;
        var apple: Math::Vec2;
        var size: const int;
    }
}
func main() -> int {
    std::print("Starting Snake Game...");
    let game := Snake::Game(Math::Vec2(Snake::DefaultWidth, Snake::DefaultHeight));
    while game.inGame {
        let dir := Math::Vec2::Origin();
        if std::random(0, 2) == 0 {
            if std::random(0, 2) == 0 {
                dir = Math::Vec2::Up();
            }
            else {
                dir = Math::Vec2::Right();
            }
        }
        else {
            if std::random(0, 2) == 0 {
                dir = Math::Vec2::Down();
            }
            else {
                dir = Math::Vec2::Left();
            }
        }
        game.move(dir);
    }
    std::print("Terminating Snake Game...");
    return 0;
}
```