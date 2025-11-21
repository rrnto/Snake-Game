import tkinter as tk
import random

# ----------------------------
# Game constants
# ----------------------------
WIDTH = 600
HEIGHT = 600
BLOCK = 20
GRID_W = WIDTH // BLOCK
GRID_H = HEIGHT // BLOCK
SPEED = 120   # movement speed in ms

class SnakeGame:
    def __init__(self, root):
        self.root = root
        self.root.title("Snake Game (Python Tkinter)")

        self.canvas = tk.Canvas(root, width=WIDTH, height=HEIGHT, bg="black")
        self.canvas.pack()

        # Bind keyboard
        root.bind("<Left>", lambda e: self.set_dir("Left"))
        root.bind("<Right>", lambda e: self.set_dir("Right"))
        root.bind("<Up>", lambda e: self.set_dir("Up"))
        root.bind("<Down>", lambda e: self.set_dir("Down"))

        # Start game
        self.reset()

    def reset(self):
        self.direction = "Right"
        self.snake = [(10, 10), (9, 10), (8, 10)]  # grid coords
        self.food = self.spawn_food()
        self.score = 0
        self.game_over = False

        # Remove restart button if exists
        if hasattr(self, "restart_btn"):
            self.restart_btn.destroy()

        self.update()

    # ----------------------------
    # Game helpers
    # ----------------------------
    def spawn_food(self):
        while True:
            x = random.randint(0, GRID_W - 1)
            y = random.randint(0, GRID_H - 1)
            if (x, y) not in self.snake:
                return (x, y)

    def set_dir(self, new):
        opposite = {"Left": "Right", "Right": "Left", "Up": "Down", "Down": "Up"}
        if new != opposite.get(self.direction):
            self.direction = new

    def move_snake(self):
        head_x, head_y = self.snake[0]

        if self.direction == "Left": head_x -= 1
        if self.direction == "Right": head_x += 1
        if self.direction == "Up": head_y -= 1
        if self.direction == "Down": head_y += 1

        new_head = (head_x, head_y)
        self.snake.insert(0, new_head)

        # Food eaten?
        if new_head == self.food:
            self.food = self.spawn_food()
            self.score += 1  # +1 like JS/C
        else:
            self.snake.pop()

    def check_collision(self):
        head = self.snake[0]

        # Wall collision
        if head[0] < 0 or head[0] >= GRID_W or head[1] < 0 or head[1] >= GRID_H:
            self.game_over = True

        # Self collision
        if head in self.snake[1:]:
            self.game_over = True

    # ----------------------------
    # Rendering
    # ----------------------------
    def draw(self):
        self.canvas.delete("all")

        # Draw food
        fx, fy = self.food
        self.canvas.create_rectangle(
            fx*BLOCK, fy*BLOCK, fx*BLOCK+BLOCK, fy*BLOCK+BLOCK, fill="red"
        )

        # Draw snake
        for x, y in self.snake:
            self.canvas.create_rectangle(
                x*BLOCK, y*BLOCK, x*BLOCK+BLOCK, y*BLOCK+BLOCK, fill="green"
            )

        # Draw score
        self.canvas.create_text(
            10, 10, text=f"Score: {self.score}", fill="white", anchor="nw"
        )

        # Game over text
        if self.game_over:
            self.canvas.create_text(
                WIDTH//2, HEIGHT//2,
                text="GAME OVER",
                fill="red",
                font=("Arial", 24)
            )

            # Restart button
            self.restart_btn = tk.Button(
                self.root, text="Restart", command=self.reset
            )
            self.restart_btn.place(x=WIDTH//2 - 40, y=HEIGHT//2 + 40)

    # ----------------------------
    # Main loop
    # ----------------------------
    def update(self):
        if not self.game_over:
            self.move_snake()
            self.check_collision()

        self.draw()
        self.root.after(SPEED, self.update)


# ----------------------------
# Run program
# ----------------------------
root = tk.Tk()
SnakeGame(root)
root.mainloop()
