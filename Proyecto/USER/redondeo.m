function [R]=redondeo(N)
decimal=N-fix(N);
if N>=0
    if decimal>=0.5
        R=fix(N)+1;
    else
        R=fix(N);
    end
else
    if decimal<=-0.5
        R=fix(N)-1;
    else
        R=fix(N);
    end
end

end

