t <!DOCTYPE html>
t     <html>
t       <head>
t         <meta content="text/html; charset=UTF-8" http-equiv="content-type">
t         <meta http-equiv="refresh" content="7; url=http://192.168.0.125/index.cgi">
t         <title>Control  Web</title>
t       </head>
t           <table style="height:174px; width=700px; border-collapse: collapse;"border="1">
t             <tbody>
t               <tr style="height:18px;width=700px;">
t                 <td style"width: 100%;height: 18px; background-color: #2e6c80; tex-aling: center:"colspan="3">
t                   <span style="color: #000000;"><strong> Control coche<br/></strong></span></td>
t               </tr>
t               <tr style="height: 18px;width=700px;">
c 1 a                 <td style="width: 100%; height: 18px; text-aling: center;"><strong> %s <br/></strong></span></td>
t                   <div style="font-weight:bold">
t               </tr>
t              <tr style="height: 32px;width=700px;">
t                 <td style="width: 400px; height: 100%; text-aling: center;">
c 1 b 					<p> Vel. Derecha: %d <br/></p></td>
t   			  <td style="width: 300px; height: 100%; text-aling: center;">
c 1 m               %s
t   					</td></tr>
t              <tr style="height: 32px;width=700px;">
t                 <td style="width: 400px; height: 100%; text-aling: center;">
c 1 c 					<p> Vel. Izquierda: %d <br/></p></td>
t   			  <td style="width: 300px; height: 100%; text-aling: center;">
c 1 n  				%s  
t   					</td></tr>
t              <tr style="height: 32px;width=700px;">
t                 <td style="width: 400px; height: 100%; text-aling: center;">
c 1 d 					<p> Posicion Servo: %d <br/></p></td>
t   			  <td style="width: 300px; height: 100%; text-aling: center;">
c 1 o  				%s  
t   					</td></tr>
t              <tr style="height: 32px;width=900px;">
t                 <td style="width: 100%; height: 100%; text-aling: center;">
c 1 f                 	<p> Sensor Luz 1: %d <br/></p>
t   					</td></tr>
t                 <td style="width: 100%; height: 100%; text-aling: center;">
c 1 g                 	<p> Sensor Luz 2: %d <br/></p>
t   					</td></tr>
t                 <td style="width: 100%; height: 100%; text-aling: center;">
c 1 h                 	<p> Promedio: %d <br/></p>
t   					</td></tr>
t                 <td style="width: 100%; height: 100%; text-aling: center;">
c 1 i                 	<p> Distancia: %d <br/></p>
t   					</td></tr>
t             </tbody>
t           </table>
t 			<p><strong>Seleccion de modo</strong></p>
t       	<form id="formulario"> 
t  			<p>
c 1 j 				<input %s name="manual" type="radio" value="manual" OnClick="submiy();"/> Modo Manual
c 1 k 				<input %s name="automatico" type="radio" value="automatico" OnClick="submiy();"/> Modo Automatico
c 1 l   			<input %s name="depuracion" type="radio" value="depuracion" OnClick="submiy();"/> Modo Depuracion
t   			</p></form>
t     </html>
.