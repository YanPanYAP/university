function buildHistogramSvg(title, stats) {
  const width = 1500;
  const height = 650;
  const margin = { top: 95, right: 40, bottom: 110, left: 60 };
  const chartWidth = width - margin.left - margin.right;
  const chartHeight = height - margin.top - margin.bottom;
  const barWidth = chartWidth / stats.frequencies.length;
  const maxValue = Math.max(...stats.frequencies.map((item) => item.frequency), 0.01);
  const gridSteps = 12;

  const bars = stats.frequencies
    .map((item, index) => {
      const barHeight = (item.frequency / maxValue) * (chartHeight - 20);
      const x = margin.left + index * barWidth + 5;
      const y = margin.top + chartHeight - barHeight;
      const labelY = y - 10;
      const textAnchor = x + barWidth / 2 - 5;

      return `
        <rect x="${x.toFixed(2)}" y="${y.toFixed(2)}" width="${Math.max(
          barWidth - 10,
          14
        ).toFixed(2)}" height="${barHeight.toFixed(2)}" fill="url(#barGradient)" stroke="#111" stroke-width="1.2" />
        <text x="${textAnchor.toFixed(2)}" y="${labelY.toFixed(
          2
        )}" class="value">${item.frequency.toFixed(3)}</text>
        <text x="${textAnchor.toFixed(2)}" y="${(margin.top + chartHeight + 35).toFixed(
          2
        )}" class="symbol">${item.symbol}</text>
      `;
    })
    .join("");

  const horizontalGrid = Array.from({ length: gridSteps + 1 }, (_, index) => {
    const value = (maxValue / gridSteps) * index;
    const y = margin.top + chartHeight - (chartHeight / gridSteps) * index;
    return `
      <line x1="${margin.left}" y1="${y.toFixed(2)}" x2="${width - margin.right}" y2="${y.toFixed(
        2
      )}" class="grid" />
      <text x="${margin.left - 12}" y="${(y + 4).toFixed(2)}" class="axis">${value.toFixed(
        2
      )}</text>
    `;
  }).join("");

  return `<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" width="${width}" height="${height}" viewBox="0 0 ${width} ${height}">
  <defs>
    <linearGradient id="bg" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" stop-color="#fafafa" />
      <stop offset="55%" stop-color="#e8e8e8" />
      <stop offset="100%" stop-color="#cfcfcf" />
    </linearGradient>
    <linearGradient id="barGradient" x1="0%" y1="0%" x2="0%" y2="100%">
      <stop offset="0%" stop-color="#2a2a2a" />
      <stop offset="100%" stop-color="#7d7d7d" />
    </linearGradient>
    <pattern id="verticalGrid" width="24" height="24" patternUnits="userSpaceOnUse">
      <path d="M 24 0 L 0 0 0 24" fill="none" stroke="#c6c6c6" stroke-width="1"/>
    </pattern>
    <style>
      .title { font: italic 700 34px Georgia, serif; fill: #202020; text-anchor: middle; }
      .axis { font: 18px 'Times New Roman', serif; fill: #333; text-anchor: end; }
      .symbol { font: 22px 'Times New Roman', serif; fill: #111; text-anchor: middle; }
      .value { font: 17px 'Times New Roman', serif; fill: #111; text-anchor: middle; }
      .grid { stroke: #b5b5b5; stroke-width: 1; stroke-dasharray: 5 4; }
    </style>
  </defs>
  <rect width="${width}" height="${height}" fill="url(#bg)" />
  <rect x="${margin.left}" y="${margin.top}" width="${chartWidth}" height="${chartHeight}" fill="url(#bg)" stroke="#8c8c8c" stroke-width="1.2"/>
  <rect x="${margin.left}" y="${margin.top}" width="${chartWidth}" height="${chartHeight}" fill="url(#verticalGrid)" opacity="0.8" />
  ${horizontalGrid}
  <text x="${width / 2}" y="56" class="title">${title}</text>
  ${bars}
  <line x1="${margin.left}" y1="${margin.top + chartHeight}" x2="${width - margin.right}" y2="${margin.top + chartHeight}" stroke="#111" stroke-width="2" />
</svg>`;
}

module.exports = { buildHistogramSvg };
