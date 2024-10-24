let img;
let cassette;

let speedSlider;
let lastTick = 0;

const r = 12;
const PIXEL_COLOR_PER_INPUT = 2; // number of each color pixel in 1 input chunk

let lightOff, colorFinal;
let colorInput = [];

function preload() {
  img = loadImage("cassette.png");
}

function setup() {
  createCanvas(800, 600);

  lightOff = color("#b8b8b8");
  lightOff.setAlpha(32);
  colorFinal = color("#894CE0");
  colorInput = ["#8D7EFC", "#BE94FF", "#CD6EFF", "#FFA9FF", "#FFF"];

  cassette = new Cassette(90, 90, 600, 376);

  speedSlider = createSlider(5, 200, 80, 10);

  textAlign(LEFT, TOP);
  textSize(16);
}

function draw() {
  background(20);
  fill("white");
  text("Press 'Q' to add joy pixels (left)", 10, 10);
  text("Press 'P' to add joy pixels (right)", 10, 30);

  if (millis() - lastTick >= speedSlider.value()) {
    cassette.tick();
    lastTick = millis();
  }
  cassette.draw();
}

function keyPressed() {
  switch (key) {
    case "p":
      cassette.legRight.addPixels();
      break;
    case "q":
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

    let spoolY = y + h / 2 - 23;
    let spoolX1 = x + 170;
    let spoolX2 = x + 420;
    this.spoolLeft = new Spool(spoolX1, spoolY, this.y + this.h);
    this.spoolRight = new Spool(spoolX2, spoolY, this.y + this.h);

    this.legLeft = new Leg(
      spoolX1,
      this.y + this.h,
      75,
      this.addPixelsLeft.bind(this)
    );
    this.legRight = new Leg(
      spoolX2,
      this.y + this.h,
      75,
      this.addPixelsRight.bind(this)
    );
  }

  addPixelsLeft(c) {
    this.spoolLeft.addPixels(c);
  }

  addPixelsRight(c) {
    this.spoolRight.addPixels(c);
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
    this.legRight.tick();
    this.spoolLeft.tick();
    this.spoolRight.tick();
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

    let m = 12; // margin
    // top row from left
    for (let i = this.x + m; i < this.x + this.w - m; i += r) {
      p.push(new Pixel(i, this.y + m));
    }
    // right side from top
    for (let i = this.y + m; i < this.y + this.h - r; i += r) {
      p.push(new Pixel(this.x + this.w - m, i));
    }
    // bottom row from right
    for (let i = this.x + this.w - m; i > this.x + m; i -= r) {
      p.push(new Pixel(i, this.y + this.h - m));
    }
    // left side from bottom
    for (let i = this.y + this.h - m; i > this.y + r; i -= r) {
      p.push(new Pixel(this.x + m, i));
    }
    return p;
  }
}

class Leg {
  constructor(cx, yTop, h, edgeCallback) {
    this.cx = cx;
    this.yTop = yTop;
    this.h = h;

    this.pixels = this.generatePixels();
    this.pixelColors = this.pixels.map((_) => lightOff);

    this.edgeCallback = edgeCallback;
    this.i = 0;
    this.queue = 0;
  }

  addPixels() {
    this.queue += colorInput.length * PIXEL_COLOR_PER_INPUT; // pixels per input
  }

  tick() {
    if (this.queue > 0) {
      this.queue--;

      // 2 pixels in a row of each color per input
      const c = colorInput[Math.floor(this.i / PIXEL_COLOR_PER_INPUT)];
      this.pixelColors.unshift(color(c));
      this.i = (this.i + 1) % (colorInput.length * PIXEL_COLOR_PER_INPUT);
    } else {
      this.pixelColors.unshift(lightOff);
    }

    // last pixel should move up onto spool
    const p = this.pixelColors.pop();
    this.edgeCallback(p);
  }

  draw() {
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
    // lmao magic numbers
    this.angle = 1.13;
    this.initialRadius = 43;
    this.radius = this.initialRadius;
    this.spacing = 10.02;
    this.numPixels = 280;

    this.pixels = this.generatePixels();
    this.pixelColors = this.pixels.map((_) => lightOff);

    this.queue = [];

    this.circleNum = 0;
    this.pixelsCounter = 0;
    this.pixelsPerCircle = 3 * PIXEL_COLOR_PER_INPUT * colorInput.length;
    this.maxCircles = 5;
  }

  addPixels(c) {
    this.queue.push(c);
  }

  tick() {
    if (this.queue.length > 0) {
      let c = this.queue.shift();
      this.pixelColors.unshift(c);
    } else {
      this.pixelColors.unshift(lightOff);
    }
  }

  draw() {
    this.pixels.forEach((pixel, i) => {
      pixel.setColor(this.pixelColors[i]);
      pixel.draw();
    });
    this.checkAndDrawCircles();
  }

  checkAndDrawCircles() {
    // Check if the last pixel in the array has reached the center of the spool
    let maxPixels = this.pixelsPerCircle * this.maxCircles;
    if (
      this.pixels[this.pixels.length - 1].color !== lightOff &&
      this.pixelsCounter < maxPixels
    ) {
      // if it has color, increment the counter
      this.pixelsCounter++;
    }

    let filledRings = floor(this.pixelsCounter / this.pixelsPerCircle);
    let pixelsInCurrentRing = this.pixelsCounter % this.pixelsPerCircle;

    // Draw rings from smallest to largest
    for (let i = 0; i <= filledRings; i++) {
      let opacity;

      if (i === filledRings) {
        opacity = map(pixelsInCurrentRing, 0, this.pixelsPerCircle, 0, 255);
      } else {
        opacity = 255;
      }

      let opacityColor = color(
        red(colorFinal),
        green(colorFinal),
        blue(colorFinal),
        opacity
      );

      noFill();
      stroke(opacityColor);
      strokeWeight(r);

      // +4 for spacing between rings
      let ringRadius = this.radius + i * (r * 2 + 4);
      circle(this.x, this.y, ringRadius);
    }
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
    circle(this.x, this.y, r);
  }
}
