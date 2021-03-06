function display_message(msg){
    document.getElementById("message").innerHTML=msg;
}

function read(file){
    return new Promise((resolve,reject)=>{
        let reader=new FileReader();
        reader.readAsText(file);
        reader.onload=function (evt){
            resolve(reader.result.split('\n'));
        }
    });
}

function to_int(val){
    const parsed=parseInt(val,10);
    if(isNaN(parsed)){
        console.log("to_int parse error");
        return -1;
    }
    return parsed;
}

function draw_rect(a,b,c,d,canvas){
    const width=canvas.width,n=10000;
    console.assert(a<=c && b<=d);
    a*=width/n;
    b*=width/n;
    c*=width/n;
    d*=width/n;
    canvas.fillRect(a,b,c-a,d-b);
    canvas.strokeRect(a,b,c-a,d-b);
}

function color_value(r,s){
    let ans=1-Math.pow(1-Math.min(r,s)/Math.max(r,s),2);
    if(s<r)ans=-ans;
    return Math.floor(ans*255);
}

function draw(n,input,output){
    console.assert(input.length==n);
    console.assert(output.length==n);
    let canvas=document.getElementById("canvas").getContext("2d");
    let cnt=0;
    canvas.width=500,canvas.height=500;
    canvas.clearRect(0,0,canvas.width,canvas.height);
    canvas.strokeRect(0,0,canvas.width,canvas.height);
    for(let i=0;i<n;i++){
        const input_line=input[i].split(' ');
        const output_line=output[i].split(' ');
        console.assert(input_line.length==3);
        console.assert(output_line.length==4);
        const x=to_int(input_line[0]),y=to_int(input_line[1]),r=to_int(input_line[2]);
        const a=to_int(output_line[0]),b=to_int(output_line[1]),c=to_int(output_line[2]),d=to_int(output_line[3]);
        if(x<a || c<=x || y<b || d<=y){
            canvas.fillStyle="silver";
            draw_rect(a,b,c,d,canvas);
            continue;
        }
        cnt++;
        let col=color_value(r,(c-a)*(d-b));
        if(col<0)canvas.fillStyle="rgb("+(-col)+","+(-col)+",255)";
        else canvas.fillStyle="rgb(255,"+col+","+col+")";
        console.log(col);
        draw_rect(a,b,c,d,canvas);
    }
    display_message("n="+n+", ok="+cnt);
}

function display(){
    let input_file = document.getElementById("input_file").files[0];
    let output_file = document.getElementById("output_file").files[0];
    Promise.all([read(input_file),read(output_file)]).then((values)=>{
        console.log("file success");
        console.log(values);
        let input=values[0],output=values[1];
        let n=to_int(input[0]);
        input.shift();
        draw(n,input,output);
    }).catch((error)=>{
        console.log("Error: "+error);
    })
}