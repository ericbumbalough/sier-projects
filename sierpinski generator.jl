using Cairo, Fontconfig, Compose

function sierpinski(n)
    if n == 0
        compose(context(), polygon([(1,1), (0,1), (1/2, 0)]))
    else
        t = sierpinski(n - 1)
        compose(context(),
                (context(1/4,   0, 1/2, 1/2), t),
                (context(  0, 1/2, 1/2, 1/2), t),
                (context(1/2, 1/2, 1/2, 1/2), t))
    end
end


for i = 0:8
    composition = compose(sierpinski(i), fill("black"))

    draw(PNG("tomato$i.png", 4cm, 8*(sqrt(3)/4)*cm), composition)
end
