i header.inc
t  <span class="text-dark"> 
c n %d.%d.%d.%d
t </span>
i mid.inc
i temp1.inc
c s 1 <h1 class="mt-1 mb-3">%g  ºC </h1>
i humedad.inc
c s 2 <h1 class="mt-1 mb-3">%g %% </h1>
i viento.inc
c s 3 <h1 class="mt-1 mb-3">%g rpm </h1>
i temp2.inc
c s 4 <h1 class="mt-1 mb-3">%g  ºC </h1>
i presion.inc
c s 5 <h1 class="mt-1 mb-3">%g %% </h1>
i umbral.inc
t <form  action="/index.cgi" method="GET">
t <h1 class="mt-1 mb-3"> 
c u %g  ºC 
t </h1>
t <div class="mb-3">
t <label class="form-label">Introduzca un nuevo umbral</label>
t <input type="number"  step="0.01" class="form-control" placeholder="f.e: 25.78" id="umbral" name="umbral">
t </div>
t <button type="submit" class="btn btn-primary">Enviar</button>
t </form>
i footer.inc
.