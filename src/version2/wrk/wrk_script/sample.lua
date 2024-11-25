-- 랜덤한 docName 생성 함수
function random_docname()
    local docnames = {
        "home", "about", "contact", "services", "products",
        "blog", "faq", "terms", "privacy", "help"
    }
    return docnames[math.random(1, #docnames)]
end

-- HTTP 요청 메시지를 작성하는 함수
function request()
    local docName = random_docname() -- 랜덤하게 docName을 선택

    local request_message = string.format(
        "GET /?docName=%s HTTP/1.1\r\n" ..
        "Host: %s\r\n" ..
        "\r\n",
        docName, wrk.host  -- wrk.host는 wrk 실행 시에 설정된 호스트값을 자동으로 사용
    )

    return wrk.format("GET", "/", nil, request_message)  -- GET 요청 반환
end
