const ENEMY_SPEED = 2;
const PLAYER_SPEED = 5;
const BULLET_DAMAGE = 5; // per level
const ENEMY_RADIUS = 120;
const PLAYER_RADIUS = 40;
const BULLET_RADIUS = 30;
const BULLET_SPEED = 5;

let enemiesKilled = 0;
let waveBegin = +new Date();
let sentWaveEnd = false;
let upcomingEnemies = [];

// Create a scene
const scene = new THREE.Scene();

// Vertex shader code
const vertexShader = `
    varying vec2 vUv;

    void main() {
        vUv = uv;
        gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
    }
`;

// Fragment shader code
const fragmentShader = `
    uniform sampler2D videoTexture;
    varying vec2 vUv;

    void main() {
        vec4 texel = texture2D(videoTexture, vUv);
        if (length(texel.xyz) < 0.1) { // Assuming green screen, adjust for blue
            discard; // Discard the pixel
        } else {
            gl_FragColor = texel; // Render the pixel as is
        }
    }
`;

const textureLoader = new THREE.TextureLoader();
const mapTexture = textureLoader.load('./map.png');

let playerMaterial = new THREE.ShaderMaterial({
    uniforms: {
        videoTexture: { value: new THREE.VideoTexture( video__player ) }
    },
    vertexShader: vertexShader,
    fragmentShader: fragmentShader,
    transparent: true // Enable transparency
}), playerGeometry = new THREE.CircleGeometry(PLAYER_RADIUS, 32);

let enemyMaterial = new THREE.ShaderMaterial({
        uniforms: {
            videoTexture: { value: new THREE.VideoTexture( video__enemy ) }
        },
        vertexShader: vertexShader,
        fragmentShader: fragmentShader,
        transparent: true // Enable transparency
    }),
    enemyGeometry = new THREE.CircleGeometry(ENEMY_RADIUS, 32);

let bulletMaterial = new THREE.ShaderMaterial({
        uniforms: {
            videoTexture: { value: textureLoader.load('./bullet.png') }//new THREE.VideoTexture( video__bullet ) }
        },
        vertexShader: vertexShader,
        fragmentShader: fragmentShader,
        side: THREE.DoubleSide,
        transparent: true
    }),
    bulletGeometry = new THREE.CircleGeometry(BULLET_RADIUS, 32);

let powerUpMaterial = new THREE.ShaderMaterial({
        uniforms: {
            videoTexture: { value: textureLoader.load('./powerup.png') }//new THREE.VideoTexture( video__bullet ) }
        },
        vertexShader: vertexShader,
        fragmentShader: fragmentShader,
        side: THREE.DoubleSide,
        transparent: true
    }),
    powerUpGeometry = new THREE.CircleGeometry(BULLET_RADIUS, 32);

mapTexture.wrapS = THREE.RepeatWrapping;
mapTexture.wrapT = THREE.RepeatWrapping;
const mapRepeat = 150; // Adjust this value to control the repetition

// Create the plane geometry for the map background
const mapGeometry = new THREE.PlaneGeometry(window.innerWidth * 500, window.innerHeight * 500);
const mapMaterial = new THREE.MeshBasicMaterial({ map: mapTexture });
const mapMesh = new THREE.Mesh(mapGeometry, mapMaterial);

// Set the position of the map background behind other objects
mapMesh.position.z = -2;

// Set the repeat factor for the texture
mapTexture.repeat.set(mapRepeat, mapRepeat);

// Create a camera
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 2000);
camera.position.z = 1000;

// Create a renderer
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);