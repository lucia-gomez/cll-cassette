let img;
let cassette;

let speedSlider;
let lastTick = 0;

let r = 12;
let lightOff, colorFinal;
let colorInput = [];

function preload() {
  img = loadImage("cassette.png");
}

function setup() {
  createCanvas(800, 600);

  lightOff = color(220, 220, 220);
  colorFinal = color(154, 54, 255);
  colorInput = ["#3b79f5", "#7b6af7", "#d9a1ff", "#ff4af9", "#ff2672"];

  cassette = new Cassette(50, 50, 600, 385);

  speedSlider = createSlider(10, 500, 100, 10);

  textAlign(LEFT, TOP);
  textSize(16);
}

function draw() {
  background(255);
  fill("black");
  text("Press 'P' to add joy pixels", 10, 10);

  if (millis() - lastTick >= speedSlider.value()) {
    // cassette.legLeft.addPixels();
    cassette.tick();
    lastTick = millis();
  }
  cassette.draw();
}

function keyPressed() {
  switch (key) {
    case "p":
      console.log("add");
      // cassette.addPixels();
      cassette.legLeft.addPixels();
      break;
  }
}

class Cassette {
  constructor(x, y, w, h) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;

    this.borderPixels = this.generateBorderPixels();
    this.borderColors = this.borderPixels.map((_) => colorFinal);

    let spoolY = y + h / 2 + 24;
    let spoolX1 = x + 179;
    let spoolX2 = x + 432;
    this.spoolLeft = new Spool(spoolX1, spoolY, this.y + this.h);
    this.spoolRight = new Spool(spoolX2, spoolY, this.y + this.h);

    this.legLeft = new Leg(
      spoolX1,
      this.y + this.h,
      100,
      150,
      this.addPixelsLeft.bind(this)
    );
    this.legRight = new Leg(
      spoolX2,
      this.y + this.h,
      100,
      150,
      this.addPixelsRight.bind(this)
    );
  }

  addPixelsLeft() {
    this.spoolLeft.addPixels();
  }

  addPixelsRight() {
    this.spoolLeft.addPixels();
  }

  tick() {
    // BORDER ANIMATION
    // this.i = (this.i + 1) % this.borderPixels.length;
    // this.borderColors.unshift(this.borderColors.pop());

    // if (this.queue > 0 && this.i % 4 === 0) {
    //   this.queue -= 4;
    //   for (let j = 0; j < 4; j++) {
    //     this.borderColors[j] = colorFinal;
    //   }
    // }

    this.legLeft.tick();
    // this.legRight.tick();
    this.spoolLeft.tick();
    // this.spoolRight.tick();
  }

  draw() {
    this.legLeft.draw();
    this.legRight.draw();

    this.spoolLeft.draw();
    this.spoolRight.draw();

    this.borderPixels.forEach((pixel, i) => {
      pixel.setColor(this.borderColors[i]);
      pixel.draw();
    });

    fill(245);
    tint(255, 64);
    image(img, this.x, this.y);
  }

  generateBorderPixels() {
    const p = [];

    // top row from left
    for (let i = this.x; i < this.x + this.w; i += r) {
      p.push(new Pixel(i, this.y));
    }
    // right side from top
    for (let i = this.y; i < this.y + this.h - r; i += r) {
      p.push(new Pixel(this.x + this.w, i));
    }
    // bottom row from right
    for (let i = this.x + this.w; i > this.x; i -= r) {
      p.push(new Pixel(i, this.y + this.h));
    }
    // left side from bottom
    for (let i = this.y + this.h; i > this.y + r; i -= r) {
      p.push(new Pixel(this.x, i));
    }
    return p;
  }
}

class Leg {
  constructor(cx, yTop, w, h, edgeCallback) {
    this.cx = cx;
    this.yTop = yTop;
    this.w = w;
    this.h = h;

    this.pixels = this.generatePixels();
    this.pixelColors = this.pixels.map((_) => lightOff);

    this.edgeCallback = edgeCallback;
    this.i = 0;
    this.queue = 0;
  }

  addPixels() {
    this.queue += 1;
  }

  tick() {
    this.i++;

    if (this.queue > 0) {
      this.queue--;
      const c = random(colorInput);
      this.pixelColors.unshift(color(c));
    } else {
      this.pixelColors.unshift(lightOff);
    }

    // last pixel should move up onto spool
    const p = this.pixelColors.pop();
    if (p.toString() != lightOff.toString()) {
      this.edgeCallback();
    }
  }

  draw() {
    stroke(0);
    fill(255);
    rect(this.cx - this.w / 2, this.yTop, this.w, this.h);

    this.pixels.forEach((pixel, i) => {
      pixel.setColor(this.pixelColors[i]);
      pixel.draw();
    });
  }

  generatePixels() {
    const p = [];
    for (let y = this.yTop + this.h; y > this.yTop; y -= r) {
      p.push(new Pixel(this.cx, y));
    }
    return p;
  }
}

class Spool {
  constructor(x, y, borderBottom) {
    this.x = x;
    this.y = y;
    this.cassetteBorderBottom = borderBottom;

    // spiral parameters
    this.angle = 0;
    this.initialRadius = 40;
    this.radius = this.initialRadius;
    this.spacing = 10.02;
    this.numPixels = 280;

    this.pixels = this.generatePixels();
    this.pixelColors = this.pixels.map((_) => lightOff);

    this.i = 0;
    this.queue = 0;
  }

  addPixels() {
    this.queue++;
  }

  tick() {
    this.i++;
    // this.pixelColors.unshift(this.pixelColors.pop());

    if (this.queue > 0) {
      this.queue--;
      this.pixelColors.unshift(colorFinal);
    } else {
      this.pixelColors.unshift(lightOff);
    }
  }

  draw() {
    this.pixels.forEach((pixel, i) => {
      pixel.setColor(this.pixelColors[i]);
      pixel.draw();
    });
  }

  generatePixels() {
    const p = [];
    let cx, cy;
    for (let i = 0; i < this.numPixels; i++) {
      cx = this.x + this.radius * cos(this.angle);
      cy = this.y + this.radius * sin(this.angle);

      p.push(new Pixel(cx, cy));

      // maintain constant distance between pixels like on LED strip
      let circumference = TWO_PI * this.radius;
      let deltaAngle = this.spacing / this.radius;
      this.angle += deltaAngle;
      this.radius += (this.spacing * deltaAngle) / TWO_PI;
    }

    // from spool to border bottom
    const numLegPixels = Math.floor((this.cassetteBorderBottom - cy) / r);
    for (let i = 0; i < numLegPixels; i++) {
      let y = cy + r * (i + 1);
      p.push(new Pixel(cx, y));
    }
    return p.reverse();
  }
}

class Pixel {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    this.color = lightOff;
  }

  setColor(c) {
    this.color = c;
  }

  draw() {
    noStroke();
    fill(this.color);
    circle(this.x, this.y, 12);
  }
}
