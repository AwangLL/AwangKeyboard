import{d as F,r as B,x as b,o as c,c as d,u as n,a as C,t as V,M as G,n as A,a5 as M,N as K,a4 as N,b as $}from"./index-9a014484.js";import{g as R}from"./index-e1cd8dc0.js";import{_ as S}from"./_plugin-vue_export-helper-c27b6911.js";const Y={key:0,class:"top"},z={key:0,style:{"margin-top":"8px"}},D={key:1,class:"top"},E={key:2,class:"top"},H=F({__name:"key",props:{x:{default:0},y:{default:0},width:{default:54},height:{default:54},color:{default:"#FFFFFF"},selected:{type:Boolean},pressed:{type:Boolean},keycode:{default:-1},disabled:{type:Boolean,default:!1}},setup(v){const e=v;M(h=>({cf421404:s,"59b0cfef":g,"31d86a45":n(p),"63ffe14a":u,"63ffe148":m,"3fd53e65":n(f),"1bfc46fa":x}));const s=e.width+"px",g=e.height+"px",u=e.x+"px",m=e.y+"px",x=e.y+2+"px",r=B(!1),i=b(()=>R(e.keycode)),f=b(()=>{var h=parseInt(e.color.substr(1,2),16),k=parseInt(e.color.substr(3,2),16),_=parseInt(e.color.substr(5,2),16),t=e.color.length<=7?255:parseInt(e.color.substr(7,2),16),y=Math.floor(h*t/255+255-t),w=Math.floor(k*t/255+255-t),I=Math.floor(_*t/255+255-t),o=y.toString(16);o.length==1&&(o="0"+o);var a=w.toString(16);a.length==1&&(a="0"+a);var l=I.toString(16);return l.length==1&&(l="0"+l),"#"+o+a+l}),p=b(()=>{var h=parseInt(e.color.substr(1,2),16),k=parseInt(e.color.substr(3,2),16),_=parseInt(e.color.substr(5,2),16),t=e.color.length<=7?255:parseInt(e.color.substr(7,2),16),y=Math.floor((h*t/255+255-t)*.8),w=Math.floor((k*t/255+255-t)*.8),I=Math.floor((_*t/255+255-t)*.8),o=y.toString(16);o.length==1&&(o="0"+o);var a=w.toString(16);a.length==1&&(a="0"+a);var l=I.toString(16);return l.length==1&&(l="0"+l),"#"+o+a+l});return(h,k)=>{var _,t,y;return c(),d("div",{class:A(["key",r.value?"key-pressed":"",v.selected?"key-selected":"",v.disabled?"disabled":"active"])},[(_=n(i))!=null&&_.labels?(c(),d("div",Y,[C("div",null,V(n(i).labels[0]),1),((t=n(i))==null?void 0:t.labels.length)>1?(c(),d("div",z,V(n(i).labels[1]),1)):G("",!0)])):(y=n(i))!=null&&y.icon?(c(),d("div",D,[C("i",{class:A("i-ic-"+n(i).icon)},null,2)])):(c(),d("div",E))],2)}}});const L=S(H,[["__scopeId","data-v-2ea12682"]]),W={class:"key-group"},X={class:"key-box"},j=F({__name:"key-group",props:{modelValue:null,keys:null,width:null,height:null,keymap:{default:()=>[]},color:{default:()=>[]},disabled:{type:Boolean,default:!1}},emits:["update:modelValue"],setup(v,{emit:e}){const s=v;M(r=>({"331242ec":s.width,"62d40e3a":s.height}));const g=b({get(){return s.modelValue},set(r){e("update:modelValue",r)}}),u=B(new Array(s.keys.length)),m=B(new Array(s.keys.length));u.value.fill(!1),m.value.fill(!1);function x(r){g.value!=-1&&(u.value[g.value]=!1),u.value[r]=!0,g.value=r}return(r,i)=>(c(),d("div",W,[(c(!0),d(K,null,N(s.keys,(f,p)=>(c(),d("div",X,[$(L,{x:f.x,y:f.y,width:f.width,height:f.height,keycode:s.keymap[p],color:s.color[p],selected:u.value[p],pressed:u.value[p],onClick:h=>x(p),disabled:s.disabled},null,8,["x","y","width","height","keycode","color","selected","pressed","onClick","disabled"])]))),256))]))}});const P=S(j,[["__scopeId","data-v-fb353948"]]);export{P as A};