close all;
clear all;
escala=100 % Modificar este factor para ajustar la amplitud de las muestras entre 0 y 255 (DAC de 8 bits)
% audiorecorder(Fs, N_bits, canales);
r = audiorecorder(8000, 16, 1);
record(r); % grabando....
pause(2.0); % 2 segundos
stop(r); % stop

play(r); % reproduce lo grabado
datos = getaudiodata(r, 'int16'); % get data as int16 array
y=datos/escala; % valores entre -127 y 128 (ojo enviamos muestras de 8 bits al DAC
figure(1)
plot(y)

y=abs(min(y))+y; % Sin valores negativos (Es necesario escribir en el DAC valores entre 0 y 255)
figure(2)
x=redondeo(y); % DAC 8 bits (ojo valores enteros!! menores que 255)
plot(x);
pause(0); % Observa la amplitud y divide y por un factor si se precisa
L=length(x)

fichero = 'audio_muestras.h';

fid=fopen(fichero, 'w');

% generamos el array,  muestras[]={}
fprintf(fid,'const uint8_t muestras[]={');
for j=1:L
    if j == L
    fprintf(fid,'%d',[x(j)]); 
    else fprintf(fid,'%d,',[x(j)]); 
    end
end
fprintf(fid,'}; \n\r');

fclose(fid);


