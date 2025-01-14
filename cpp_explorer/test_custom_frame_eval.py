# import torch
import custom_eval_frame


custom_eval_frame.enable_custom_eval()


def foo():
    print("Hello from foo!")


def mse(x, y):
    z = (x - y) ** 2
    return z

foo()

# x = torch.randn(200)
# y = torch.randn(200)
mse(2, 3)