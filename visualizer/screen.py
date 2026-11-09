## shrink the window to fit the screen
## https://stackoverflow.com/questions/3949844/how-can-i-get-the-screen-size-in-tkinter
def fit(fig):
    try:
        window = fig.canvas.manager.window
        w, h = window.winfo_screenwidth(), window.winfo_screenheight()
        ppi = fig.dpi * window.winfo_fpixels("1i") / 96
    except AttributeError:
        return 1
    width, height = fig.get_size_inches()
    ## leaving some rooms for title bar and whatnot
    new_w, new_h = min(width, 0.9 * w / ppi), min(height, 0.8 * h / ppi)
    fig.set_size_inches(new_w, new_h)
    return min(new_w / width, new_h / height)
