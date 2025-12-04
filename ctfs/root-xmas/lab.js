const snowContainer = document.querySelector(".snow");
const SNOWFLAKE_COUNT = 70;

function createSnowflake() {
  if (!snowContainer) return;
  const snowflake = document.createElement("span");
  snowflake.className = "snowflake";
  snowflake.textContent = Math.random() > 0.5 ? "❅" : "✻";
  const size = 0.6 + Math.random() * 0.9;
  const duration = 10 + Math.random() * 12;
  const delay = Math.random() * -20;
  const horizontalOffset = (Math.random() - 0.5) * 80;

  snowflake.style.left = `${Math.random() * 100}vw`;
  snowflake.style.fontSize = `${size}rem`;
  snowflake.style.opacity = `${0.4 + Math.random() * 0.5}`;
  snowflake.style.animationDuration = `${duration}s`;
  snowflake.style.animationDelay = `${delay}s`;
  snowflake.style.setProperty("--x-offset", `${horizontalOffset}px`);

  snowContainer.appendChild(snowflake);

  setTimeout(
    () => {
      snowflake.remove();
    },
    (duration + Math.abs(delay)) * 1000,
  );
}

if (snowContainer) {
  for (let i = 0; i < SNOWFLAKE_COUNT; i++) {
    createSnowflake();
  }
  setInterval(() => createSnowflake(), 800);
}

async function loadProfile() {
  const response = await fetch("/api/profile").catch(() => null);
  if (!response || !response.ok) {
    window.location.href = "/login.html";
    return;
  }
  const contentType = response.headers.get("content-type") || "";
  if (!contentType.includes("application/json")) {
    window.location.href = "/login.html";
    return;
  }
  const data = await response.json().catch(() => null);
  if (!data || !data.username) {
    window.location.href = "/login.html";
    return;
  }

  const usernameElement = document.getElementById("username");
  if (usernameElement) {
    usernameElement.textContent = data.username;
  }
  document.getElementById("welcomeUsername").textContent = data.username;
  if (data.csrfToken) {
    window.csrfToken = data.csrfToken;
  }

  if (data.sessionExpiresAt) {
    startCountdown(data.sessionExpiresAt);
  }

  // @ElfDev : Dont forget to remove this debug snippet after testing. (DevSecOops)
  try {
    const packageResponse = await fetch("/api/package");
    if (packageResponse.ok) {
      const packageJson = await packageResponse.json();
      console.debug("[package] package.json exposed:", packageJson);
    }
  } catch (error) {
    console.debug("[package] package.json unavailable");
  }
}

async function loadStats() {
  const response = await fetch("/api/stats");
  const data = await response.json();

  const statsDiv = document.getElementById("statsContent");
  statsDiv.textContent = "";

  if (data.status === "building") {
    const loading = document.createElement("p");
    loading.className = "loading";
    loading.textContent = data.message || "Generating stats… Please wait.";
    statsDiv.appendChild(loading);
    setTimeout(loadStats, 5000);
    return;
  }

  const fields = [
    ["Gifts Wrapped", data.giftsWrapped || 0],
    ["Toys Made", data.toysMade || 0],
    ["Letters Read", data.lettersRead || 0],
    ["Efficiency", `${data.efficiency || 0}%`],
  ];

  for (const [label, value] of fields) {
    const item = document.createElement("div");
    item.className = "stat-item";

    const name = document.createElement("span");
    name.className = "stat-label";
    name.textContent = `${label}:`;

    const val = document.createElement("span");
    val.className = "stat-value";
    val.textContent = value;

    item.append(name, val);
    statsDiv.appendChild(item);
  }
}

async function logout() {
  const headers = {};
  if (window.csrfToken) {
    headers["X-CSRF-Token"] = window.csrfToken;
  }
  await fetch("/api/logout", { method: "POST", headers });
  window.location.href = "/login.html";
}

loadProfile();
loadStats();
setInterval(loadStats, 30000);

const countdownElement = document.getElementById("sessionCountdown");
function startCountdown(expirationTimestamp) {
  if (!countdownElement) return;
  const update = () => {
    const remaining = Math.max(0, expirationTimestamp - Date.now());
    const minutes = Math.floor(remaining / 60000);
    const seconds = Math.floor((remaining % 60000) / 1000);
    countdownElement.textContent = `${minutes}m ${seconds.toString().padStart(2, "0")}s`;
    if (remaining <= 0) {
      clearInterval(timer);
      countdownElement.textContent = "0m 00s";
      logout();
    }
  };
  update();
  const timer = setInterval(update, 1000);
}
