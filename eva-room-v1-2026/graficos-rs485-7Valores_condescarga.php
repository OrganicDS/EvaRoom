<?php
    $serie = isset($_GET['serie']) ? $_GET['serie'] : ''; // Obtén el valor de 'serie' o usa una cadena vacía si no está definido
    $filename = 'procesando_7Valores.php?serie=' . $serie; // Crea el nombre del archivo
?>
<!DOCTYPE html>
<html lang="es">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Eva grow </title>
    <!-- Importar chart.js -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.0/dist/js/bootstrap.bundle.min.js"></script>
    <link rel="stylesheet"  type="text/css" href="./css/estilos_7Valores.css">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Comfortaa:wght@700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@48,400,0,0" />
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.15.4/css/all.css">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css" />

   
</head>

<body>

    
   
    
    <script>
        const filename = "<?php echo $filename; ?>"; // Usa la variable PHP en JavaScript
        
        const source = new EventSource(filename);
        
        source.onmessage = (event) => {
            const data = JSON.parse(event.data);
            document.getElementById('humSustrato').textContent = data.humSustrato;
            document.getElementById('tempSustrato').textContent = data.tempSustrato;
            document.getElementById('conductivity').textContent = (data.conductivity)+"µS/cm";
            document.getElementById('conductivity_ppm').textContent = (data.conductivity * 0.7).toFixed(0)+"ppm";
            document.getElementById('pH').textContent = data.pH;
            document.getElementById('nitrogen').textContent = data.nitrogen;
            document.getElementById('phosphorus').textContent = data.phosphorus;
            document.getElementById('potassium').textContent = data.potassium;
            document.getElementById('tempAmbiente').textContent = data.tempAmbiente;
            document.getElementById('humAmbiente').textContent = data.humAmbiente;
        };
        window.onload = function() {
    
    
}
var lastReceivedTimestamp;
    var checkConnectionInterval;

    function startCheckingConnection() {
      checkConnectionInterval = setInterval(checkConnection, 1000);
    }

    function checkConnection() {
      var connectionStatusElement = document.getElementById("connectionStatus");
      if (Date.now() - lastReceivedTimestamp > 5000) {
        connectionStatusElement.textContent = " Offline";
        connectionStatusElement.className = "fas fa-times";
        connectionStatusElement.style.color = "red";
      } else {
        connectionStatusElement.textContent = " Online";
        connectionStatusElement.className = "fas fa-check";
        connectionStatusElement.style.color = "green";
      }
    }

    function onDataReceived(data) {
      lastReceivedTimestamp = Date.now();
      checkConnection();
    }

    function showCurrentDateTime() {
      var currentDateElement = document.getElementById("currentDate");
      var currentDate = new Date();
      currentDateElement.textContent = currentDate.toLocaleString();
    }

    window.onload = function() {
      startCheckingConnection();
      setInterval(showCurrentDateTime, 1000);
    }
    </script>
 <body>   
<div class="div-conteiner">

    <div class="contiene">
<div class="header">
    <div class="logo">
      <img src="./img/logo_n.png" class="imagen" style="width: 100px;">
    </div>
    <div class="status">
      <p style="margin:0px; font-size: 10px;">Estado: <i id="connectionStatus"></i></p>
      <p class="current-date" id="currentDate" style="margin:0px; font-size: 10px;"></p>
    </div>
  </div>

        
                <div class="datos">
    <div class="numero_left" id="caja1">
        <div class="titulos"><i class="fas fa-tint"></i> Humedad en sustrato</div><span id="humSustrato"></span>%
    </div>

    <div class="numero_left" id="caja2">
        <div class="titulos"><i class="fas fa-thermometer-half"></i> Temp. en sustrato</div><span id="tempSustrato"></span>°C
    </div>

    <div class="numero_left" id="caja3">
        <div class="titulos"><i class="fas fa-vial"></i> pH</div><span id="pH"></span>
    </div>

    <div class="numero_left" id="caja4">
    <div class="titulos"><i class="fas fa-bolt"></i> Conductividad</div>
    <div id="conductivity" style="font-size: 13px; "></div>
    <div id="conductivity_ppm" style="font-size: 13px;"></div> 
</div>

    <div class="numero_left" id="caja8">
        <div class="titulos"><i class="fas fa-thermometer-half"></i> Temp. ambiente</div><span id="tempAmbiente"></span>°C
    </div>

    <div class="numero_left" id="caja9">
        <div class="titulos"><i class="fas fa-tint"></i> Hum. ambiente</div><span id="humAmbiente"></span>%
    </div>
    <div class="numero_right" id="caja5">
        <div class="titulos"><i class="fas fa-leaf"></i> Nitrógeno(N)</div><span id="nitrogen"></span>mg/kg
    </div>

    <div class="numero_right" id="caja6">
        <div class="titulos"><i class="fas fa-leaf"></i> Fósforo(P)</div><span id="phosphorus"></span>mg/kg
    </div>

    <div class="numero_right" id="caja7">
        <div class="titulos"><i class="fas fa-leaf"></i> Potasio(K)</div><span id="potassium"></span>mg/kg
    </div>
</div>
</div>



<div class="container text-center">

<p>Consulta los datos de los últimos:</p>
</div>
<div class="container">
    <div class="row">
    <div class="col-3 text-center py-2">
    <button id="last1Days" class="myButton">24 hs</button>
        </div>
        <div class="col-3 text-center py-2">
    <button id="last10Days" class="myButton">10 días</button>
        </div>
        <div class="col-3 text-center py-2">
    <button id="last30Days" class="myButton">30 días</button>
        </div>
        <div class="col-3 text-center py-2">
    <button id="last3Months" class="myButton">3 meses</button>
        </div>
        
</div>
</div>

<div class="contenedor_chart">
<canvas id="myChart" height="350"></canvas>
</div>


<div class="chacontenedor_chart">
<canvas id="myChartnpk" height="350"></canvas>
</div>
<!-- ↓↓↓ ponelo debajo del/los canvas de los gráficos ↓↓↓ -->
<div id="evaExportToolbar" class="container text-center">
    <button id="evaBtnCsvBoth" class="myButto">⬇️ Descargar CSV (ambos gráficos)</button>
    
</div>


<footer class="footer mt-auto py-3 bg-light">
    <div class="container text-center">
        <img src="./img/logo_pie.png" alt="Eva grow" class="img-fluid" style="max-width: 200px;"> <!-- Ajusta el max-width como lo necesites -->
    </div>
</footer>


      <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
      
      <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
      
      <script src="https://cdn.jsdelivr.net/npm/chartjs-adapter-date-fns/dist/chartjs-adapter-date-fns.bundle.min.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/hammer.js/2.0.8/hammer.min.js"></script>
      <script src="https://cdnjs.cloudflare.com/ajax/libs/chartjs-plugin-zoom/1.2.1/chartjs-plugin-zoom.min.js"></script>
      
      


<script>

var numSerie = "<?php echo $serie ?>";


let myChart;  // Variable para guardar la instancia del gráfico
let myChartnpk;
function loadChartData(timePeriod) {
    let url;
    if (timePeriod === '10days') {
        url = "./datos-rs485.php?serie="+numSerie+"&tiempo=10dias";
    } 
    else if (timePeriod === '30days') {
        url = "./datos-rs485.php?serie="+numSerie+"&tiempo=30dias";
    } 
    else if (timePeriod === '3months') {
        url = "./datos-rs485.php?serie="+numSerie+"&tiempo=90dias";
    } 
    else if (timePeriod === '1Day') {
        url = "./datos-rs485.php?serie="+numSerie+"&tiempo=1dia";
    }
    

    fetch(url)
        .then(response => response.json())
        .then(data => {
            if (myChart) {
                // Si el gráfico ya existe, destrúyelo para crear uno nuevo
                myChart.destroy();
            }

            if (myChartnpk) {
                // Si el gráfico ya existe, destrúyelo para crear uno nuevo
                myChartnpk.destroy();
            }
            const sensorData = data;
            // Extracción de datos para el gráfico
            const labels = sensorData.map(item => item.reading_time);
            const humSustrato = sensorData.map(item => item.humSustrato);
            const tempSustrato = sensorData.map(item => item.tempSustrato);
            const humAmbiente = sensorData.map(item => item.humAmbiente);
            const tempAmbiente = sensorData.map(item => item.tempAmbiente);
            const pH = sensorData.map(item => item.pH);
            const maxYValue = 1; // Define el valor que desees
            let riegosData = sensorData.map(dato => dato.riego ? maxYValue : null);

            const conductivity_ppm = sensorData.map(item => item.conductivity_ppm);
            const nitrogen = sensorData.map(item => item.nitrogen);
            const phosphorus = sensorData.map(item => item.phosphorus);
            const potassium = sensorData.map(item => item.potassium);


            const ctx = document.getElementById('myChart').getContext('2d');
            myChart = new Chart(ctx, {
                
            type: 'line',
            data: {
                labels: labels,
                datasets: [
                    {
                            label: 'Humedad Sustrato',
                            data: humSustrato,
                            fill: true,
						    tension: 0.5,						
                            borderColor: "#1f77b4",
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            backgroundColor: "rgba(112, 255, 19, 0.2)"
                        },
                        {
                            label: 'Temperatura Sustrato',
                            data: tempSustrato,
                            borderColor: '#ff7f0e',
                            fill: false,
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            tension: 0.5
                            
                        },
                        {
                            label: 'Humedad Ambiente',
                            data: humAmbiente,
                            borderColor: '#7f7f7f',
                            fill: false,
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            borderDash: [5,5]
                        },
                        {
                            label: 'Temperatura Ambiente',
                            data: tempAmbiente,
                            borderColor: '#bcbd22',
                            fill: false,
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            borderDash: [5,5]
                        },
                        {
                            label: 'pH',
                            data: pH,
                            yAxisID: 'y1', 
                            borderColor: '#d62728',
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            fill: false,
                            tension: 0.5
                            
                        },
                        

                ]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    y: {
                        beginAtZero: true,
                        min: 0,
                        max: 100,
                        ticks: {
                            font: {
                        size: 14  // Cambia según tus necesidades
                    },
                            beginAtZero: true,
                        }
                    },
                    y1: {
                        min: 3,
                        max: 9,
                        position: 'right',
                        scaleLabel: {
                            display: true,
                            labelString: 'pH sustrato'
                        },
                    },
                    yriego: { // Este es un segundo eje Y específico para los riegos
                        position: 'right',
                        ticks: {
                            font: {
                        size: 14  // Cambia según tus necesidades
                    },
                            max: 1,
                            min: 0,
                            display: false, // Oculta las marcas si no deseas ver 0 y 1 en el lado derecho
                        },
                        
                    },
                    x: {
                        barPercentage: 1,     // Controla el ancho de la barra (menor significa barras más delgadas)
                        categoryPercentage: 0.8,
                        type: 'time',
                        time: {
                            displayFormats: {
                                unit: 'day'
                            },
                            ticks: {
                                callback: function(val, index) {
                                    return index % 10 === 0 ? this.getLabelForValue(val) : '';
                                }
                            }
                        }
                    },
                }
            }
        });
                //... GRAFICO NPK

            const ctx2 = document.getElementById('myChartnpk').getContext('2d');
            myChartnpk = new Chart(ctx2, {
                
            type: 'line',
            data: {
                labels: labels,
                datasets: [
                    {
                            label: 'Nitrogeno',
                            data: nitrogen,
                            fill: true,
						    tension: 0.5,						
                            borderColor: "#9467bd",
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            
                        },
                        {
                            label: 'Fósforo',
                            data: phosphorus,
                            borderColor: '#8c564b',
                            fill: false,
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            tension: 0.5
                            
                        },
                        {
                            label: 'Potasio',
                            data: potassium,
                            borderColor: '#e377c2',
                            fill: false,
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            
                        },
                        {
                            label: 'Conductividad PPM',
                            data: conductivity_ppm,
                            borderColor: '#2ca02c ',
                            fill: false,
                            borderWidth:1.5,
                            pointRadius: 0, // <-- Establece esto a 0 para quitar los puntos
                            type: 'line',
                            borderDash: [5,5],
                            yAxisID: 'yppm',
                        },
                       

                ]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                scales: {
                    y: {
                        beginAtZero: true,
                        suggestedMax: 2500, // o más según tus pruebas
                        ticks: {
                        stepSize: 150
                                }
                        
                    },
                    
                    yppm: { // Este es un segundo eje Y específico para los riegos
                        position: 'right',
                        beginAtZero: true,
                        suggestedMax: 2500, // o más según tus pruebas
                        ticks: {
                        stepSize: 150
                                }
                        
                    },
                    x: {
                        barPercentage: 1,     // Controla el ancho de la barra (menor significa barras más delgadas)
                        categoryPercentage: 0.8,
                        type: 'time',
                        time: {
                            displayFormats: {
                                unit: 'day'
                            },
                            ticks: {
                                callback: function(val, index) {
                                    return index % 10 === 0 ? this.getLabelForValue(val) : '';
                                }
                            }
                        }
                    },
                }
            }
        });

            });
}
loadChartData('1Day');
document.getElementById('last1Days').addEventListener('click', () => {
    loadChartData('1Day');
});
document.getElementById('last10Days').addEventListener('click', () => {
    loadChartData('10days');
});

document.getElementById('last30Days').addEventListener('click', () => {
    loadChartData('30days');
});

document.getElementById('last3Months').addEventListener('click', () => {
    loadChartData('3months');
});

// Obtener todos los botones por la clase "myButton"
const buttons = document.querySelectorAll('.myButton');

// Función para eliminar la clase 'active' de todos los botones
function deactivateAllButtons() {
    buttons.forEach(btn => {
        btn.classList.remove('active');
    });
}

// Agregar un listener de evento para el clic a cada botón
buttons.forEach(btn => {
    btn.addEventListener('click', function() {
        // Desactiva todos los botones
        deactivateAllButtons();
        // Activa el botón clickeado
        btn.classList.add('active');
    });
});

window.evaCharts = window.evaCharts || [];
window.evaCharts.push(myChart);   // primer gráfico
window.evaCharts.push(myChart2);  // segundo gráfico


</script>
</div>


<script>
// ===== util =====
function downloadText(filename, text, mime){
  const blob = new Blob([text], {type: mime+';charset=utf-8'});
  const url  = URL.createObjectURL(blob);
  const a    = document.createElement('a');
  a.href = url; a.download = filename;
  document.body.appendChild(a);
  a.click(); a.remove();
  URL.revokeObjectURL(url);
}
function buildFileName(prefix){
  const ns = window.evaNserie || window.Nserie || 'serie';
  const d = new Date(), y=d.getFullYear(), m=String(d.getMonth()+1).padStart(2,'0'), dd=String(d.getDate()).padStart(2,'0');
  return `${prefix}_${ns}_${y}${m}${dd}`;
}
function getAllCharts(){
  const found = [];
  if (Array.isArray(window.evaCharts)) for (const ch of window.evaCharts) if (ch?.data) found.push(ch);
  if (window.myChart?.data)  found.push(window.myChart);
  if (window.myChart2?.data) found.push(window.myChart2);
  try {
    if (typeof Chart?.getChart === 'function') {
      document.querySelectorAll('canvas').forEach(cv => {
        const ch = Chart.getChart(cv);
        if (ch?.data) found.push(ch);
      });
    }
  } catch(e){}
  // de-dup
  return Array.from(new Set(found));
}

// ===== exportar UN chart =====
function csvFromChart(chart){
  const labels = chart.data.labels || [];
  const datasets = chart.data.datasets || [];
  if (!labels.length || !datasets.length) return '';

  // Encabezados: Fecha + cada dataset
  const headers = ['Fecha', ...datasets.map(ds => (ds.label || 'serie'))];
  const lines = [headers.join(',')];

  for (let i = 0; i < labels.length; i++){
    const row = [String(labels[i]).replace(/,/g,'.')];
    for (const ds of datasets){
      const v = Number(ds.data?.[i]);
      row.push(Number.isFinite(v) ? v : '');
    }
    lines.push(row.join(','));
  }
  return lines.join('\n');
}

// ===== exportar DOS charts combinados =====
function csvFromTwoCharts(chartA, chartB){
  if (!chartA?.data || !chartB?.data) return '';

  const labelsA = chartA.data.labels || [];
  const labelsB = chartB.data.labels || [];

  // Unión de labels en orden de A (y agregamos las de B que no estén)
  const labelSet = new Set(labelsA.map(String));
  const labelsUnion = [...labelsA.map(String)];
  labelsB.forEach(lb => { const s = String(lb); if (!labelSet.has(s)) { labelSet.add(s); labelsUnion.push(s); } });

  // Columnas: Fecha + datasets de A + datasets de B (con sufijo)
  const headA = (chartA.data.datasets || []).map(ds => (ds.label || 'serie') + ' [G1]');
  const headB = (chartB.data.datasets || []).map(ds => (ds.label || 'serie') + ' [G2]');
  const headers = ['Fecha', ...headA, ...headB];
  const lines = [headers.join(',')];

  // Índices por label para acceso rápido
  const idxA = new Map(labelsA.map((lb, i) => [String(lb), i]));
  const idxB = new Map(labelsB.map((lb, i) => [String(lb), i]));

  for (const lab of labelsUnion){
    const row = [lab.replace(/,/g,'.')];

    // valores de A
    for (const ds of (chartA.data.datasets || [])){
      const i = idxA.get(lab);
      const v = Number(i != null ? ds.data?.[i] : NaN);
      row.push(Number.isFinite(v) ? v : '');
    }
    // valores de B
    for (const ds of (chartB.data.datasets || [])){
      const i = idxB.get(lab);
      const v = Number(i != null ? ds.data?.[i] : NaN);
      row.push(Number.isFinite(v) ? v : '');
    }

    lines.push(row.join(','));
  }

  return lines.join('\n');
}

// ===== listeners =====
document.getElementById('evaBtnCsvG1')?.addEventListener('click', () => {
  const charts = getAllCharts();
  if (!charts.length) return alert('No encuentro el gráfico 1');
  const csv = csvFromChart(charts[0]);
  if (!csv) return alert('Gráfico 1 sin datos');
  downloadText(buildFileName('eva_grafico1') + '.csv', csv, 'text/csv');
});

document.getElementById('evaBtnCsvG2')?.addEventListener('click', () => {
  const charts = getAllCharts();
  if (charts.length < 2) return alert('No encuentro el gráfico 2');
  const csv = csvFromChart(charts[1]);
  if (!csv) return alert('Gráfico 2 sin datos');
  downloadText(buildFileName('eva_grafico2') + '.csv', csv, 'text/csv');
});

document.getElementById('evaBtnCsvBoth')?.addEventListener('click', () => {
  const charts = getAllCharts();
  if (charts.length < 2) return alert('Necesito dos gráficos para combinar.');
  const csv = csvFromTwoCharts(charts[0], charts[1]);
  if (!csv) return alert('No hay datos combinados para exportar.');
  downloadText(buildFileName('eva_ambos') + '.csv', csv, 'text/csv');
});
</script>

</body>

</html>