let wasm, player1, player2, start;
let app = document.getElementById('app');
let ctx = app.getContext('2d');

const PaddleSide = {
  Right: 'Right',
  Left: 'Left'
}
const RightPaddleKeys = {
  u: 'u',
  j: 'j',
}
const LeftPaddleKeys = {
  w: 'w',
  s: 's',
}
class Player {
  constructor(element) {
    this.element = document.getElementById(element);
    this.setScore(0);
  }
  setScore(score) {
    this.element.textContent = score;
  }
}

function make_environment(...envs) {
  return new Proxy(envs, {
    get(target, prop, receiver) {
      for (let env of envs) {
        if (env.hasOwnProperty(prop)) {
          return env[prop];
        }
        return (...args) => { console.error("Not implemented: " + prop, args) }
      }
    }
  });
}

function step() {
  let d = new Date();
  if (!start || d.getTime() - start > 1 / 60) {
    start = d.getTime();
    wasm.step();
    updatePlayers();
    render();
  }
  window.requestAnimationFrame(step);
}

function updatePlayers() {
  player1.setScore(wasm.getScore1());
  player2.setScore(wasm.getScore2());
}
function render() {
  const array = new Uint8ClampedArray(wasm.memory.buffer, wasm.pixels, wasm.getWidth() * wasm.getHeight() * 4);
  const image = new ImageData(array, wasm.getWidth());
  ctx.putImageData(image, 0, 0);
}

WebAssembly.instantiateStreaming(fetch("./output/main.wasm"), {
  "env": make_environment()
}).then(w => {
  wasm = w.instance.exports;
  app.width = wasm.getWidth();
  app.height = wasm.getHeight();
  player1 = new Player('player1');
  player2 = new Player('player2');
  window.requestAnimationFrame(step);
})

document.onkeydown = function(key) {
  if (RightPaddleKeys.hasOwnProperty(key.key) || LeftPaddleKeys.hasOwnProperty(key.key)) {
    switch (key.key) {
      case 'u':
        wasm.moveRightUp();
        break;
      case 'j':
        wasm.moveRightDown();
        break;
      case 'w':
        wasm.moveLeftUp();
        break;
      case 's':
        wasm.moveLeftDown();
        break;
      default:
        console.error("Unreachable");
    }
  }
}
